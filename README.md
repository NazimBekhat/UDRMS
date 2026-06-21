# UDRMS — University Dormitory & Restaurant Management System

OOP course project (ENSIA, 2025-2026) — C++17 core with a Qt6 GUI.

## Features
- Role-based access: Student, Staff, Administrator, SystemAdmin
- Dormitory, room, and restaurant management
- File-based data persistence (backup/load)
- Custom exception hierarchy for validation and error handling

## Architecture
- `core/` — pure C++ domain logic (no Qt dependency)
- `ui/` — Qt6 widgets calling core directly (no controller layer)
- `data/` — runtime save files (gitignored except for `.gitkeep`)
- `docs/` — UML diagram and design notes

## Installation & Build
Requires Qt6 (Core, Widgets) and CMake ≥ 3.19.

```bash
git clone https://github.com/NazimBekhat/UDRMS.git
cd UDRMS
mkdir build && cd build
cmake ..
cmake --build .
./UDRMS
```

Or open `CMakeLists.txt` directly in Qt Creator and run from there.

## Test accounts (seeded in `main.cpp`)
| Username | Password | Role |
|---|---|---|
| nazim_bekhat | nazim | Student |
| john_doe | pass | Staff |
| jane_doe | pass | Administrator |
| root | pass | SystemAdmin |

## Permissions
| Role | Capabilities |
|---|---|
| Student | View rooms, view menu |
| Staff | View rooms, edit restaurant menu |
| Administrator | Assign/remove students from rooms, create Staff/Student accounts |
| SystemAdmin | All of the above, plus create Administrator accounts, backup/load data |

## Author
Nazim Bekhat — ENSIA, 1st year
📧 abderrahmane-nazim.bekhat@ensia.edu.dz