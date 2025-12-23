# 🌐 Social Network Analysis & XML Processing Tool

A robust **C++ application** designed to **parse, validate, and analyze** social network data stored in **XML format**.  
This project features a dual interface:

- 🖥️ **Graphical User Interface (GUI)** built with **Qt**
- ⚡ **Command Line Interface (CLI)** for fast batch processing

---

## 📋 Project Overview

This tool performs advanced operations on XML datasets representing **users, posts, and followers**.  
It constructs a **Social Network Graph** to perform analytics (such as finding the most influential user) and generates **visual representations** of the network.

---

## ✨ Key Features

### 🧩 1. XML Utilities
- ✅ **Validation:** Checks XML consistency (balanced tags) and detects errors  
- 🛠️ **Error Correction:** Automatically attempts to fix invalid XML syntax  
- 🎨 **Formatting:**  
  - Minified XML ➝ Prettified (indented)  
  - Prettified XML ➝ Minified (single-line)  
- 🔄 **Conversion:** Converts XML data to **JSON** format  

---

### 📦 2. Data Compression
- 🗜️ **Compress:** Reduces file size using custom binary compression (`.comp` files)  
- 📤 **Decompress:** Restores original XML files from compressed data  

---

### 🧠 3. Social Network Analysis (Graph Algorithms)
- 🖼️ **Network Visualization:** Generates graphical representations using **Graphviz** (`.png` output)  
- ⭐ **Influencer Analysis:**  
  - Most Active User  
  - Most Influential User (based on graph connectivity)  
- 🤝 **Suggestions:** Recommends new users to follow  
- 🔗 **Mutual Followers:** Finds shared connections between users  
- 🔍 **Search:** Search posts by **Word** or **Topic** using efficient parsing  

---
## 🗂️ Project Structure
```
XML-Network-Editor/
│
├── GUI/
│   ├── main.cpp
│   ├── mainwindow.cpp
│   ├── mainwindow.h
│   ├── mainwindow.ui
│   ├── fileviewer.cpp
│   ├── fileviewer.h
│   └── fileviewer.ui
│
├── Includes/
│   ├── CLI_h.hpp
│   ├── Formater.hpp
│   ├── Graphs.hpp
│   ├── Posts.hpp
│   ├── Reader.hpp
│   ├── SocialNetwork.hpp
│   ├── User.hpp
│   └── visualize.hpp
│
├── source/
│   ├── Cli/
│   │   ├── CLI.cpp
│   │   └── CLI_inputParser.cpp
│   │
│   ├── Data structures/
│   │   ├── Graphs.cpp
│   │   ├── Posts.cpp
│   │   └── User.cpp
│   │
│   └── Services/
│       ├── compress.cpp
│       ├── decompress.cpp
│       ├── JsonConverter.cpp
│       ├── Prettifyer.cpp
│       ├── Reader.cpp
│       ├── SocialNetwork.cpp
│       ├── vaild.cpp
│       ├── visualize.cpp
│       └── XML_Minifying.cpp
│
├── input/
│   └── data.xml
│
├── output/
    ├── network.dot
    ├── network.png
    ├── minified.xml
    └── compressed.comp

```
---
## 👥 Team Members

| Name | ID | Role |
| :--- | :--- | :--- |
| **Amr Ashraf Hussien** | [2201048] | 🎨 Social Network Part1 & Gui  |
| **Eyad Tarek Nagy** | [2200512] | 🧠 Xml Pretfier |
| **Youssef Ahmed Mohammed** | [2200405] | 📄Graph Architect & Parse XML |
| **Mohammed Hamada Hassan** | [2200820] | ⚙️ SocialNetwork Part 2 & Report |
| **Abdullah Mohammed Ahmed** | [2200423] | 🎨 GUI & Integration |
| **Mohammed Walid Abd Elmohsen** | [2200243] | 🧠 Xml To Json Converter |
| **Mahmoud Shabaan Mohammed** | [2200676] | 📄 XML Minfier & Compressor & Decompressor |
| **Seif Mohammed Hassan** | [2200929] | ⚙️ CLI Implementation |
| **Mohammed Ehab Mohammed** | [2201067] | 🧠 Xml Validator  |
| **Ahmed Mahmoud El morsy** | [2200725] | 📄 Report |

---

### 🎓 Submitted to
**Prof. Islam Elmadah**
**Eng.Fady Fargallah**
📘 *Course: Data Structures & Algorithms*

---

## ⚙️ Prerequisites

To build and run this project, you need:

1. 🧑‍💻 **C++ Compiler**  
   *(MinGW, G++, or MSVC — C++11 or higher)*  
2. 🖥️ **Qt 6** *(For GUI functionality)*  
3. 📊 **Graphviz** *(For network visualization)*  
   - 🪟 **Windows Note:** Must be installed and added to **System PATH**

---

## 🚀 How to Build

### 🖱️ Option 1: Using Qt Creator (GUI)
1. Open `DSA_project.pro` in **Qt Creator**
2. Run **qmake** to generate the Makefile
3. Click **Build** (🔨) or **Run** (▶️)

---

### 💻 Option 2: Using Command Line (CLI Only)
If you do not need the GUI, compile the CLI tool using **g++**:

```bash
g++ source/Cli/CLI.cpp source/Cli/CLI_inputParser.cpp source/Services/*.cpp "source/Data structures/*.cpp" -I source -o myApp
