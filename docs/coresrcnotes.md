# UDRMS — Implementation Notes (.cpp companion)

Companion to `UDRMS_Project_Notes.md`. Covers what each `.cpp` actually does, plus implementation-level details and one bug spotted during this review.

---

## ⚠️ One real bug spotted (not yet fixed — flagging only, not worth fixing tonight)

**`Room::setCapacity()` checks the wrong variable:**
```cpp
void Room::setCapacity(int capacity){
    if (roomNumber<=0) throw InvalidCapacityException(capacity);   // BUG: checks roomNumber, not capacity
    this->capacity = capacity;
}
```
Should be `if (capacity <= 0) throw InvalidCapacityException(capacity);`. As written, this setter never actually validates the new capacity value — it validates the *existing* `roomNumber` instead, which is always already valid by the time this runs. Low real-world impact (setCapacity likely isn't called from the UI), but worth a one-line fix post-submission.

---

## Room.cpp — validation pattern

Static helper functions used inside the constructor's initializer list, so validation can run *before* the member is initialized while still using initializer-list syntax (rather than throwing from inside the constructor body after partial construction):

```cpp
static int validateRoomNumber(int roomNumber){
    if (roomNumber <= 0) throw InvalidRoomNumberException(roomNumber);
    return roomNumber;
}
static int validateCapacity(int capacity){
    if (capacity <= 0) throw InvalidCapacityException(capacity);
    return capacity;
}
Room::Room(int roomNumber, int capacity)
    : roomNumber(validateRoomNumber(roomNumber)), capacity(validateCapacity(capacity)) {}
```

`addStudent()` checks in this order (most specific failure first):
1. `student->getInRoom()` → `StudentAlreadyInRoomException` (student already has a room anywhere)
2. Duplicate-pointer scan in `students` → same exception (defensive, shouldn't trigger if #1 works)
3. `isFull()` → `RoomFullException`

`removeStudent(int studentID)` — linear scan, throws `StudentNotInRoomException` if no match found after the loop (no silent no-op).

---

## SharedRoom.cpp — capacity range

```cpp
SharedRoom::SharedRoom(int roomNumber, int capacity): Room(roomNumber, capacity){
    if (capacity <= 2 || capacity >= 5) throw InvalidCapacityException(capacity);
}
```
Valid range: **3 or 4** only. Thrown *after* `Room`'s base constructor already ran (base is constructed before derived body executes) — meaning the object is technically fully formed before this throw fires, but since the exception propagates immediately and the (now partially-used) object is never returned to the caller, no leak/UB results — this is standard, safe C++ exception-during-construction behavior.

---

## Dormitory.cpp — ownership and exception behavior

| Method | Behavior |
|---|---|
| `addRoom(Room*)` | Throws `InvalidRoomNumberException` on duplicate room number (fixed late — originally silently returned) |
| `removeRoom(int)` | Throws `RoomNotEmptyException` if room still occupied; silently returns if room not found (minor, defensive-only gap — UI never passes an invalid number since it's chosen from a populated list) |
| `addStudentToRoom(Student*, Room*)` | Throws `InvalidRoomNumberException` if room doesn't belong to this dormitory, `RoomFullException` if at capacity |
| `~Dormitory()` | Deletes every `Room*` — relies on `Room`'s virtual destructor to correctly destroy the concrete subtype (`SingleRoom`/`DoubleRoom`/`SharedRoom`) through the base pointer |

`std::remove` + `erase` idiom used in `removeRoom` — `std::remove` doesn't shrink the vector, just moves "kept" elements to the front and returns an iterator to where garbage starts; `erase` then actually shrinks it.

---

## University.cpp — key methods

```cpp
User* University::findUserByID(int userID){
    for (User* user: users){
        if (Student* s = dynamic_cast<Student*>(user)){
            if (s->getStudentID() == userID) return s;
        } else if (Staff* st = dynamic_cast<Staff*>(user)){
            if (st->getStaffID() == userID) return st;
        } else if (Administrator* a = dynamic_cast<Administrator*>(user)){
            if (a->getAdminID() == userID) return a;
        }
        // SystemAdmin has no ID — intentionally unmatched
    }
    return nullptr;
}
```
This is the fixed version — originally only checked `Student`, silently failing for Staff/Administrator lookups. Now tries each concrete subtype in turn via `dynamic_cast`.

```cpp
void University::clearAll(){
    for (User* u : users) delete u;
    users.clear();
    dormitories.clear();
}
```
Added specifically to fix the `loadData()` crash — without this, every `Load` operation would *append* freshly-read users/dormitories on top of whatever was already in memory, causing duplicate entries and (separately) a dangling-pointer crash on the logged-in user once this method started deleting the array contents. Called as the very first thing inside `loadData()`, after confirming the file opens successfully (so a failed load doesn't wipe existing data for nothing).

`addDormitory(Dormitory&& dormitory)` takes an rvalue reference specifically because `Dormitory`'s copy constructor is deleted (shallow-copy would double-delete `Room*` pointers) — so the only way to get a `Dormitory` into the vector is by moving it.

---

## SystemAdmin.cpp — backup() / loadData()

**File format**: pipe-delimited plain text, one entity per line, prefixed by a type tag:
```
UNIVERSITY|<name>
STUDENT|<id>|<fullName>|<academicYear>|<inRoom>
STAFF|<id>|<fullName>|<department>
ADMIN|<id>|<fullName>
SYSADMIN|<username>|<fullName>
DORMITORY|<name>
ROOM|<type>|<roomNumber>|<capacity>|<studentID1>|<studentID2>|...
RESTAURANT|<breakfast>|<lunch>|<dinner>
```

**Load order matters:**
1. `DORMITORY` line creates a new `Dormitory`, and `currentDorm` is set to point at it (`&uni.getDormitories().back()`) — needed because subsequent `ROOM`/`RESTAURANT` lines don't repeat the dormitory name, so the loader has to remember "which dormitory am I currently filling in."
2. `ROOM` lines: students are looked up via `findUserByID` (must already exist in `uni.getUsers()` — meaning all `STUDENT` lines must appear *before* any `ROOM` line that references them in the file). Each `addStudent()` call is wrapped in its own try/catch so one bad/duplicate student doesn't abort the whole file load — it just logs to `std::cerr` and continues.
3. Students loaded with `inRoom=false` regardless of what's saved in the file, then `addStudent()` (called when adding them to a room later in the same load) sets `inRoom=true` correctly — avoids a chicken-and-egg state mismatch.

**Why `getDormitories()` needs both a `const` and non-`const` overload**: `backup()` only reads (`const` version sufficient), but `loadData()` needs to call `.back()` and get a *mutable* reference to keep filling in rooms/restaurant data on the dormitory it just created — hence the non-const overload exists specifically for `loadData()`'s use case.

---

## Exceptions.cpp — message construction

Every exception builds its message in the **member-initializer list**, forwarding a fully-formed `std::string` straight to `UDRMSException`'s constructor (which forwards again to `std::runtime_error`):

```cpp
RoomFullException::RoomFullException(int roomNumber, int capacity)
    : UDRMSException("Room: " + std::to_string(roomNumber) + " is full (capacity: " + std::to_string(capacity) + ")")
{}
```

No exception class has a body — all the work happens in the initializer list. This is consistent across all 7 subclasses.

---

## test.cpp — status

Entirely commented out (`// ...` block-style, every line prefixed). This was the original console-only validation harness used before the Qt UI existed — confirmed all core logic (room capacity limits, duplicate prevention, exception throwing, backup/load round-trip) before any GUI code was written. Left in the repo as a historical reference / fallback compile target; not part of the active build (`CMakeLists.txt` doesn't reference it).

---

## User.cpp / Student.cpp / Staff.cpp / Administrator.cpp — straightforward

All four follow the same shape: constructor forwards `username`/`password`/`fullName` to `User`'s constructor via the initializer list, then initializes the subclass-specific field(s) (`studentID`+`academicYear`, `staffID`+`departement`, `adminID`, or nothing extra for `SystemAdmin`). Each implements `getRole()` returning a hardcoded string literal matching its class name — these strings are exactly what `loadData()`/UI role-checks compare against, so they must stay in sync if ever renamed (e.g. `"Administrator"`, not `"Admin"`, even though the file format uses `ADMIN|` as its tag).
