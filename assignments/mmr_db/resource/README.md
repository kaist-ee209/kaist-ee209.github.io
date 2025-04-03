# Assignment 3: Game Ranking System
This project implements a MMR database to manage game rankings, with three implementations:
- **Hash Table (`ht`)**
- **Binary Search Tree (`bst`)**
- **Integrated Implementation (`integrated`)**

## 📁 Directory Structure
```
├── src/ # Source files (mmr_db implementations and header)
├── test/  # Test functions
├── Makefile # Build configuration
├── readme # Documentation (Task 4)
├── STUDENT_ID # Your student ID (single line)
└── README.md  # This file
```
----------
## 🛠️ Build Instructions
To build the project, use the `make` command with one of the following options:
  ```bash
  make ht          # Hash Table Implementation
  make bst 		 # Binary Search Tree Implementation
  make integrated  # Integrated Implementation
```
Each command will generate an executable named:
```
test_mmr_db
```
----------
**✅ Run Tests**
After building the project, run the test executable:
```
./test_mmr_db
```
When prompted, enter one of the following keywords to run the corresponding test suite:
- ht — for Hash Table tests
- bst — for Binary Search Tree tests
- integrated — for Integrated tests
----------

**📦 Submit Assignment**
To create your submission package, ensure your STUDENT_ID file contains **only your student ID** (e.g., 20251234) and EthicsOath.pdf is placed in the root directory of the project. Then, run:
```
make submit
```
This will create a compressed file (submission format) named like 20251234_assign3.tar.gz containing:
- Source files under src/
- readme
- EthicsOath.pdf