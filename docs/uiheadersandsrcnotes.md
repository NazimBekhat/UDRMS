# UDRMS — UI Layer Notes (Qt screens)

Companion to the core/.cpp notes. Covers screen responsibilities, signal/slot wiring, and one leftover issue spotted during review.

---

## ⚠️ One thing worth a 10-second fix (not blocking, but technically incorrect)

**`mainwindow.cpp` has a dead, duplicate `applyShadow` function at the bottom of the file:**
```cpp
void applyShadow(QWidget* widget)
{
    auto* shadow = new QGraphicsDropShadowEffect(widget);
    shadow->setBlurRadius(8);
    shadow->setOffset(0, 2);
    shadow->setColor(QColor(0, 0, 0, 160));
    widget->setGraphicsEffect(shadow);
}
```
This is leftover from an earlier attempt, before `uihelpers.h` existed as the shared, `inline` version. `mainwindow.cpp` never calls this function and doesn't include `uihelpers.h`, so it currently compiles fine in isolation — but having two functions with the identical signature (`applyShadow(QWidget*)`), one `inline` (in the header, included by 3 other `.cpp` files) and one non-`inline` with external linkage (here), is technically an ODR (One Definition Rule) violation. In practice GCC/the linker usually resolves this silently by preferring the strong symbol, so it likely won't cause a visible build failure — but it's not guaranteed-safe behavior. **Safe fix, if you have 30 seconds: just delete this whole function block from `mainwindow.cpp`** — it's unused dead code regardless.

---

## Screen-by-screen summary

### LoginScreen
- Constructor takes `University*`, stores it, calls `applyShadow` on both line edits.
- `onLoginClicked()`: reads username/password as `QString`, converts via `.toStdString()`, calls `university->findUserByUsername()`. Short-circuit check `user == nullptr || !user->login(password)` — order matters, avoids dereferencing a null pointer.
- `clearFields()` — public method, called by `MainWindow::onLogout()` so the next person doesn't see stale input.
- Emits `loginSucceeded(User*)` on success — carries the actual logged-in object, not just a role string, so downstream screens get both identity and data in one signal.

### DashboardScreen
- `setUser(User*)` gates three things by role: Backup/Load buttons (SystemAdmin only), Create User button (Administrator or SystemAdmin).
- `getBackupFilePath()` — static free function, **not** a member — builds an absolute path next to the running executable via `QCoreApplication::applicationDirPath()`, creates the `data/` folder if missing via `QDir::mkpath`. This replaced an earlier hardcoded-path version and a broken relative-path version; this is the final, portable form.
- `onLoadClicked()` captures `currentUser`'s username into a local `QString` **before** calling `loadData()` — necessary because `loadData()` calls `University::clearAll()` internally, which deletes the very `User*` `currentUser` points to. Capturing the username first (a value copy, not a pointer) survives the deletion; emitting `dataLoaded(username)` afterward lets `MainWindow` re-fetch a fresh, valid `User*` by that name.

### RoomManagementScreen
The most complex screen — combines room management, dormitory creation, room creation, and restaurant-menu editing in one place (a deliberate scope-saving decision rather than building 2 more separate screens).

**State tracked:**
- `currentDormitoryIndex` — index into `university->getDormitories()`, not a pointer (vector elements can move/reallocate, index is safer given `populateRoomList()` always rebuilds fresh).
- `currentRoom` — raw pointer, set by `onRoomSelected`, reset to `nullptr` every time `populateRoomList()` runs (since the list was just wiped and rebuilt, any previous selection is stale).
- `currentUser` — set via `setUser()`, drives all the permission checks (`canEditMenu()`, the inline `dynamic_cast` checks in `onAssignClicked`/`onRemoveClicked`/`onAddDormitoryClicked`/`onAddRoomClicked`).

**Permission pattern repeated across 4 slots** (`onAssignClicked`, `onRemoveClicked`, `onAddDormitoryClicked`, `onAddRoomClicked`):
```cpp
if (!(dynamic_cast<Administrator*>(currentUser) || dynamic_cast<SystemAdmin*>(currentUser))) {
    QMessageBox::warning(this, "Permission Denied", "You don't have permission to do this.");
    return;
}
```
This is a **defensive backstop**, not the primary gate — the primary gate is `setUser()` hiding the relevant buttons entirely for unauthorized roles. This check only matters if something could trigger the slot without the button being visible (it currently can't, in this codebase) — included anyway as good practice.

**`onAddRoomClicked()` — exception safety detail worth noting:**
```cpp
Room* newRoom = nullptr;
try {
    if (type == "Single Room") newRoom = new SingleRoom(roomNumber);
    ...
    dorm.addRoom(newRoom);
    ...
} catch (const UDRMSException& e) {
    delete newRoom;   // avoid leak if addRoom throws or construction throws after allocation
    QMessageBox::warning(this, "Add Room Failed", e.what());
}
```
If `new SharedRoom(roomNumber, capacity)` itself throws (invalid capacity), `newRoom` is still `nullptr` at that point (the assignment never completed) — so `delete nullptr` in the catch block is a safe no-op. If construction succeeds but `dorm.addRoom(newRoom)` throws (duplicate room number), `newRoom` holds a valid, never-inserted pointer — the `delete` in the catch block is what prevents a memory leak in that specific path.

**Why `addDormitoryButton`/`addRoomButton` use `QInputDialog` instead of a custom form:** `QInputDialog::getText`/`getItem`/`getInt` are quick, single-purpose built-in Qt popups — avoided building two more full `.ui` screens for what's just 1-3 input fields each. Reasonable scope-vs-time tradeoff given the deadline.

### CreateUserScreen
- `setUser(User*)` populates `roleComboBox` based on creator's role — `SystemAdmin` sees Administrator/Staff/Student, `Administrator` sees only Staff/Student. Calls `onRoleChanged(0)` immediately after populating, since adding items to a combo box doesn't reliably auto-fire `currentIndexChanged` for the first entry.
- `onRoleChanged(int)` toggles `extraFieldLineEdit`'s visibility — shown for Student (academic year) and Staff (department), hidden for Administrator (no equivalent field).
- `onCreateClicked()` validation order: empty-field check → numeric-ID check → **duplicate-username check** → construct the right subclass based on role string → `university->addUser(newUser)`.
- Field-clearing after success — five explicit `.clear()` calls, no loop (small enough fixed set that a loop wouldn't simplify much).

### MainWindow
- Owns all four screens as members, constructs them once in the constructor, adds each to `ui->stackedWidget`, never destroys/recreates them — navigation is purely about which one is "current," not creating/destroying screens per visit.
- `loggedInUser` — tracks who's logged in at the `MainWindow` level (separate from each screen's own `currentUser` copy) specifically so `onRequestCreateUser()` and `onDataLoaded()` can push the correct user to screens reached *after* the dashboard, without needing to re-derive it from elsewhere.
- `onDataLoaded(QString username)` — the fix for the load-crash bug: re-finds the user by username (now a *different* `User*` object than before the load, since `clearAll()` deleted the old one), updates `loggedInUser`, and re-pushes to both `dashboardScreen` and `roomManagementScreen` via `setUser()`.
- `requestRoomManagement` and `requestRestaurantMenu` (two separate signals from `DashboardScreen`) both connect to the **same** slot, `onRequestRoomManagement` — intentional, since the restaurant menu view was folded into `RoomManagementScreen` rather than getting its own screen.

---

## Signal/slot wiring map (full picture)

```
LoginScreen::loginSucceeded(User*)        → MainWindow::onLoginSucceeded
DashboardScreen::requestRoomManagement()  → MainWindow::onRequestRoomManagement
DashboardScreen::requestRestaurantMenu()  → MainWindow::onRequestRoomManagement   (same slot, by design)
DashboardScreen::requestLogout()          → MainWindow::onLogout
DashboardScreen::requestCreateUser()      → MainWindow::onRequestCreateUser
DashboardScreen::dataLoaded(QString)      → MainWindow::onDataLoaded
RoomManagementScreen::requestBack()       → MainWindow::onRequestBackToDashboard
CreateUserScreen::requestBack()           → MainWindow::onRequestBackToDashboard
```

Every "back" path converges on `onRequestBackToDashboard` — both `RoomManagementScreen` and `CreateUserScreen` only ever lead back to the dashboard, never directly to each other or to login (logout is the only path back to login, via `DashboardScreen::requestLogout`).
