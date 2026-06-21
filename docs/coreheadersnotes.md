# UDRMS — Project Review Notes

Personal reference notes for reviewing the project. Covers core architecture, class responsibilities, exception hierarchy, key design decisions, and bugs fixed during development.

---

## 1. Core Class Hierarchy

### User hierarchy
```
User (abstract)
├── Student
├── Staff
├── Administrator
└── SystemAdmin   (inherits directly from User, NOT from Administrator)
```

**User.h**
```cpp
class User{
    private:
        string username;
        string password;
        string fullName;
    public:
        User(const string& username, const string& password,const string& fullName);
        virtual ~User()=default;

        virtual bool login(const string& inputPassword);
        virtual string getRole() const = 0;   // pure virtual

        void setUsername(const string& username);
        string getUsername() const;
        void setPassword(const string& password);
        void setFullName(const string& fullName);
        string getFullName() const;
};
```
- `login()` is virtual but never overridden — base implementation (`inputPassword == password`) covers all 4 roles.
- `getRole()` is pure virtual — forces every subclass to implement it, returns role as a string (`"Student"`, `"Staff"`, `"Administrator"`, `"SystemAdmin"`).

**Administrator.h**
```cpp
class Administrator: public User{
    private:
        int adminID;
    public:
        Administrator(const string& username, const string& password, int adminID, const string& fullName);
        int getAdminID() const;
        void setAdminID(int adminID);
        virtual string getRole() const override;
};
```

**Staff.h**
```cpp
class Staff: public User{
    private:
        int staffID;
        string departement;
    public:
        Staff(const string& username, const string& password, int staffID, const string& departement, const string& fullName);
        virtual string getRole() const override;
        void setStaffID(int staffID);
        int getStaffID() const;
        void setDepartement(const string& departement);
        string getDepartement() const;
};
```

**Student.h**
```cpp
class Student: public User{
    private:
        int studentID;
        string academicYear;
        bool inRoom=false;
    public:
        Student(const string& username, const string& password, int studentID, const string& fullName, const string& academicYear, bool inRoom=false);
        virtual string getRole() const override;
        void display() const;
        void setStudentID(int studentID);
        int getStudentID() const;
        void setAcademicYear(const string& academicYear);
        string getAcademicYear() const;
        void setInRoom(bool inRoom);
        bool getInRoom() const;
};
```
- `inRoom` flag prevents multi-room assignment. Must be reset to `false` when constructing from a loaded file, then `addStudent()` sets it correctly afterward.

**SystemAdmin.h**
```cpp
class SystemAdmin: public User{
    public:
        SystemAdmin(const string& username, const string& password, const string& fullName);
        virtual string getRole() const override;
        void backup(University& uni, const string& filename);
        void loadData(University& uni, const string& filename);
};
```
- No ID field — only role-holder with backup/load privileges.

---

### Room hierarchy
```
Room (abstract)
├── SingleRoom   (capacity = 1, hardcoded)
├── DoubleRoom   (capacity = 2, hardcoded)
└── SharedRoom   (capacity = 3-4, validated — throws outside range)
```

**Room.h**
```cpp
class Room{
    private:
        int roomNumber;
        int capacity;
        vector<Student*> students;   // association, NOT ownership
    public:
        Room(int roomNumber, int capacity);
        virtual ~Room()=default;
        void setRoomNumber(int roomNumber);
        int getRoomNumber() const;
        void setCapacity(int capcity);
        int getCapacity() const;
        const std::vector<Student*>& getStudents() const;
        void addStudent(Student* student);
        void removeStudent(int studentID);
        virtual string getType() const = 0;   // pure virtual
        void displayStudentsInRoom();
        bool isFull() const;
        bool isEmpty() const;
};
```
- `vector<Student*> students` — Room does NOT own the Student objects (association, not composition). University owns Users.

---

## 2. Composition Structure

```
University
├── vector<User*> users          (owns — deleted in ~University)
└── vector<Dormitory> dormitories (owns by value)
        └── Dormitory
            ├── Restaurant restaurant   (owned by value, 1:1)
            └── vector<Room*> rooms     (owns — deleted in ~Dormitory)
                    └── Room
                        └── vector<Student*> students  (association only, NOT owned)
```

**Dormitory.h** — key points:
- Copy constructor/assignment **deleted** (shallow copy would double-delete `Room*` pointers)
- Move constructor/assignment **defaulted** (needed for `vector<Dormitory>` to work — `addDormitory` takes `Dormitory&&`)
- Destructor deletes every `Room*` in `rooms`

```cpp
class Dormitory{
    private:
        string name;
        Restaurant restaurant;
        vector<Room*> rooms;
    public:
        Dormitory(const string& name);
        string getName() const;
        void setName(const string& name);
        const Restaurant& getRestaurant() const;
        Restaurant& getRestaurant();
        void addRoom(Room* room);            // throws InvalidRoomNumberException on duplicate
        void removeRoom(int roomNumber);      // throws RoomNotEmptyException / InvalidRoomNumberException
        void addStudentToRoom(Student* student, Room* room);  // throws RoomFullException / InvalidRoomNumberException
        Room* getRoom(int roomNumber);
        const vector<Room*>& getRooms() const;
        ~Dormitory();
        Dormitory(const Dormitory&)=delete;
        Dormitory& operator=(const Dormitory&)=delete;
        Dormitory(Dormitory&&) = default;
        Dormitory& operator=(Dormitory&&) = default;
};
```

**University.h**
```cpp
class University{
    private:
        string name;
        vector<User*> users;
        vector<Dormitory> dormitories;
    public:
        University(const string& name);
        ~University();   // deletes every User*
        string getName() const;
        void setName(const string& name);
        const vector<User*>& getUsers() const;
        const vector<Dormitory>& getDormitories() const;
        vector<Dormitory>& getDormitories();   // non-const overload needed for loadData()
        void addUser(User* user);
        void clearAll();                       // wipes users + dormitories — added to fix loadData() bug
        void removeUser(User* user);
        void addDormitory(Dormitory&& dormitory);
        User* findUserByID(int userID);
        User* findUserByUsername(const std::string& username);
        University(const University&)=delete;
        University& operator=(const University&)=delete;
};
```

**Restaurant.h** — simple value class, no pointers, default-copyable
```cpp
class Restaurant{
    private:
        string breakFastMenu;
        string lunchMenu;
        string dinnerMenu;
    public:
        Restaurant(const string& breakFastMenu="", const string& lunchMenu="", const string& dinnerMenu="");
        void setBreakFastMenu(const string& breakFastMenu);
        string getBreakFastMenu() const;
        void setLunchMenu(const string& lunchMenu);
        string getLunchMenu() const;
        void setDinnerMenu(const string& dinnerMenu);
        string getDinnerMenu() const;
        void displayMenu() const;
};
```

---

## 3. Exception Hierarchy

```
std::runtime_error
└── UDRMSException (base — forwards message to runtime_error)
    ├── RoomFullException             (roomNumber, capacity)
    ├── InvalidCapacityException      (attemptedCapacity)
    ├── InvalidRoomNumberException    (attemptedRoomNumber) — also reused for duplicate room/dorm names
    ├── StudentAlreadyInRoomException (studentID)
    ├── StudentNotInRoomException     (studentID)
    ├── FileException                 (filename)
    └── RoomNotEmptyException         (roomNumber)
```

**UDRMSException.h**
```cpp
class UDRMSException: public std::runtime_error{
    public:
        UDRMSException(const std::string message);
};
```

**Where each is thrown:**
| Exception | Thrown in |
|---|---|
| `RoomFullException` | `Room::addStudent()`, `Dormitory::addStudentToRoom()` |
| `StudentAlreadyInRoomException` | `Room::addStudent()` (inRoom check fires first, then duplicate-pointer scan) |
| `StudentNotInRoomException` | `Room::removeStudent()` — no match found |
| `InvalidCapacityException` | `Room`/`SharedRoom` constructors, `setCapacity()` |
| `InvalidRoomNumberException` | `Room` constructor, `setRoomNumber()`, `Dormitory::addRoom()` (duplicate), `Dormitory::addStudentToRoom()` (room not found) |
| `FileException` | `SystemAdmin::backup()`/`loadData()` on file-open failure |
| `RoomNotEmptyException` | `Dormitory::removeRoom()` — room still occupied |

---

## 4. Qt UI Layer (Architecture)

- **Single `QMainWindow`** with `QStackedWidget` for screen switching — no separate dialogs.
- **No controller layer** — UI screens call `University`/`Dormitory`/`Room` methods directly.
- **Exception handling at UI boundary**: caught per-slot, shown via `QMessageBox::warning(this, title, e.what())`.
- **Designer-based UI** (`.ui` files via Qt Creator's Form Class wizard).

### Screens built
| Screen | Responsibility |
|---|---|
| `LoginScreen` | Credential check via `University::findUserByUsername` + `User::login()`. Emits `loginSucceeded(User*)`. |
| `DashboardScreen` | Role-gated buttons: Backup/Load (SystemAdmin only), Create User (Admin/SysAdmin), Logout (everyone). Shows welcome text with name+role. |
| `RoomManagementScreen` | Dormitory selector, room list with live occupancy, Assign/Remove student (Admin/SysAdmin), Add Room, Add Dormitory, restaurant menu view+edit (Staff/Admin/SysAdmin). |
| `CreateUserScreen` | Role-aware creation form. Dropdown options depend on creator's role (SystemAdmin → Admin/Staff/Student; Administrator → Staff/Student). |

### Permission matrix
| Creator role | Can create |
|---|---|
| `SystemAdmin` | `Administrator`, `Staff`, `Student` |
| `Administrator` | `Staff`, `Student` |
| `Staff` / `Student` | nothing |

| Role | Capabilities |
|---|---|
| `Student` | View rooms, view menu (read-only) |
| `Staff` | View rooms, edit restaurant menu |
| `Administrator` | Add dormitories/rooms, assign/remove students, create Staff/Student |
| `SystemAdmin` | Everything + create Administrator, backup/load |

---

## 5. Key Bugs Found & Fixed (chronological)

1. **`using std::string`/`using namespace std` in headers** — violates standing rule (no `using` in headers). Found in `User.h`, `University.h`. Left as-is in some places due to time, flagged as tech debt.
2. **`findUserByID` only matched `Student`** — `dynamic_cast<Student*>` chain skipped Staff/Administrator entirely. Fixed with a `dynamic_cast` chain trying each subclass type.
3. **`Dormitory::addStudentToRoom` silently `return`ed** instead of throwing on full room / invalid room number. Fixed to throw `RoomFullException` / `InvalidRoomNumberException`.
4. **`Dormitory::removeRoom` same silent-return bug** on occupied room. Fixed to throw `RoomNotEmptyException` (new exception added).
5. **`Dormitory::addRoom` allowed duplicate room numbers** — missing `throw` keyword was typo'd as just constructing-and-discarding the exception object. Fixed by adding `throw`.
6. **`University::addDormitory` allowed duplicate dormitory names** — no core-level check; fixed at UI level (loop + check before calling `addDormitory`).
7. **`data/.gitkeep` ignored by `.gitignore`** — `data/*` rule caught `.gitkeep` itself. Fixed with `git add -f` once, no `.gitignore` change needed since `data/*` only ignores *contents*, not files explicitly added.
8. **Qt Designer `objectName` mismatches** — password `QLineEdit` and its label had swapped names (`PasslineEdit` vs `passwordLineEdit`), causing the code to read the label's text instead of the input field. Recurred a few times across screens — always fixed by checking Object Inspector names directly against code.
9. **Duplicate `setUser` definition** in `dashboardscreen.cpp` — pasted as an addition instead of a replacement, caused a redefinition compile error.
10. **`MainWindow` variable/class name collision** — `DashboardScreen = new DashboardScreen(...)` used the class name instead of the lowercase member variable, causing a syntax error.
11. **Critical: `loadData()` crash** — `clearAll()` deletes every `User*` in `university`, including whoever is currently logged in (since all screens share the same `University*` by reference). After a Load, every screen's `currentUser` pointer became dangling, causing a crash. **Fixed** by emitting a `dataLoaded(username)` signal after a successful load; `MainWindow` re-finds the fresh `User*` by username and re-pushes it to every screen via `setUser()`.
12. **Backup/load path fragility** — originally a relative path (`"../../data/backup.txt"`), broke when the executable was launched from a different working directory (e.g. a fresh clone). **Fixed** using `QCoreApplication::applicationDirPath()` to build an absolute path next to the running executable, with automatic `data/` folder creation via `QDir::mkpath`.
13. **UML diagram inaccuracies** — `SystemAdmin` incorrectly shown inheriting from `Administrator` instead of `User`; `Student` box incorrectly duplicated inherited `fullName` attribute; `findUserByUsername` missing from `University`; exception hierarchy entirely absent from diagram; `backup()`/`loadData()` incorrectly listed on `Administrator` instead of `SystemAdmin`. All corrected.

---

## 6. Known Limitations (documented in README)

- Single-user desktop session — no concurrent multi-user access.
- Passwords stored and compared in plaintext (no hashing) — acceptable for course scope.
- No duplicate-room-number check used a slightly mismatched exception type (`InvalidRoomNumberException` reused for "duplicate" rather than a dedicated exception) — pragmatic choice given time constraints.

---

## 7. Test Accounts (seeded in `main.cpp`)

| Username | Password | Role |
|---|---|---|
| nazim_bekhat | nazim | Student |
| john_doe | pass | Staff |
| jane_doe | pass | Administrator |
| root | pass | SystemAdmin |

---

## 8. Final Test Checklist (all passed)

1. Wrong login credentials → warning, no crash
2. Login per role (all 4 accounts) → correct dashboard each time
3. Backup/Load visibility → SystemAdmin only
4. Backup → success, file written
5. Load → success, data restored, no crash (post-fix)
6. Assign to full room → `RoomFullException`
7. Assign duplicate (already in another room) → `StudentAlreadyInRoomException`
8. Remove student not in that room → `StudentNotInRoomException`
9. Non-numeric student ID → input validation warning
10. No room selected → "No Room Selected" warning
11. Role gating — Student: read-only everywhere
12. Role gating — Staff: menu edit only
13. Role gating — Administrator: room/dorm management + create Staff/Student
14. Role gating — SystemAdmin: full access
15. Create User — duplicate username, empty fields, invalid ID all caught
16. Add Room — duplicate room number caught
17. Add Dormitory — duplicate name caught
