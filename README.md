# STASH VCS 🚀

**STASH VCS** is an educational project implementing a version control system.  
The project is developed in C++ using CMake for building and Doxygen for documentation.

---

## Key Features ⚙️

- **Branch Management:** Create, switch, and delete branches.
- **Merging Changes:** Merge your different branch progress.
- **Commit Changes:** Track and save changes in the repository.
- **Revert Changes:** Roll back to previous versions of files.
- **Logging:** View the history of changes.

---

## Installation 🛠️

### Requirements
- C++17
- CMake (version 3.10.2 or higher)
- Doxygen (for generating documentation)

### Installation Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/liltymoor/stash-vcs
2. Navigate to the project directory:
   ```bash
   cd stash-vcs
   ```
3. Build the project using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

---

## Usage 🖥️
All stash commands can be provided with -d / --description key to see what do they do.
### Example Commands
- Stage files (You also may use regex instead -f "<regex>"):
  ```bash
  ./stash add -f "..."
  ```
- Create a new commit:
  ```bash
  ./stash commit -m "..."
  ```
- Checkout (create or migrate) to new branch:
  ```bash
  ./stash checkout -b branch_name
  ```
- Revert to commit (specify hash with -h):
  ```bash
  ./stash revert_to -h 12312... 
  ```
-- Help (specify hash with -h):
  ```bash
  ./stash help
  ```
-- Merge (specify branch with -b):
  ```bash
  ./stash merge -b branch_name
  ```

---

## Documentation 📚

Project documentation is available [here](https://fr2eof.github.io/stash-vcs-docs/html/files.html)

---

## Demo Video 🎥

[![Maybe I will place the video here in future](https://via.placeholder.com/600x400)](https://github.com/liltymoor/stash-vcs/)

---

## Roadmap 🗺️

### Planned Features
- **Improved Branching:** Support for advanced branching strategies.
- ~~**Conflict Resolution:** Tools for resolving merge conflicts.~~
- **Remote Repositories:** Support for remote repository synchronization.
- **User Interface:** A graphical interface for easier interaction. (??)

---

## Contributing 🤝

We welcome contributions! Please read our [Contributing Guidelines](CONTRIBUTING.md) to get started.

---

## License 📜

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Badges 🛡️

![Build Status](https://github.com/liltymoor/stash-vcs/actions/workflows/build.yml/badge.svg)
![License](https://img.shields.io/badge/License-MIT-blue.svg)

---

## Acknowledgments 🙏

- **ITMO University** for the inspiration and support.
- **Git** for being the gold standard in version control systems.
- **Doxygen** for making documentation generation seamless.

---

## Contact 📧

For questions or feedback, feel free to reach out:  
- **Email:** timur.chelter@mail.ru or ilya.ivanov@mail.ru
- **GitHub Issues:** [Open an Issue](https://github.com/liltymoor/stash-vcs/issues)

