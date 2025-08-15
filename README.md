The Rated R Superstar
# 🔐 Chat TCP con Cifrado Híbrido RSA/AES

## 📌 Descripción
Este proyecto implementa una aplicación de chat cliente-servidor en **C++** usando **TCP** y cifrado híbrido:
- 🔑 **RSA-2048** para intercambio seguro de claves.
- 🛡 **AES-256-CBC** para cifrado simétrico de los mensajes.

Permite que dos usuarios se comuniquen de forma segura, intercambiando mensajes cifrados en tiempo real.

---

## 🚀 Características
- 📡 Conexión TCP cliente-servidor.
- 🔑 Generación de par de claves RSA (2048 bits) para cada instancia.
- 🔄 Intercambio de claves públicas entre cliente y servidor.
- 📦 Cifrado de la clave AES con la clave pública RSA del peer.
- 🛡 Comunicación cifrada con AES-256-CBC.
- 💬 Bucle de chat en paralelo (envío y recepción simultánea).
- 💻 Compatible con Windows (Winsock2 + OpenSSL).

---

## 📂 Estructura del Proyecto
```
├── Client.h / Client.cpp        # Lógica del cliente
├── Server.h / Server.cpp        # Lógica del servidor
├── NetworkHelper.h / .cpp       # Funciones auxiliares de red (TCP)
├── CryptoHelper.h / .cpp        # Funciones auxiliares de criptografía (RSA/AES)
├── Prerequisites.h              # Includes y defines comunes
├── main.cpp                     # Punto de entrada
└── README.md                    # Documentación del proyecto
```

---

## 🛠️ Requisitos
- ⚙️ **Compilador C++17 o superior**
- 🖥 **Windows** (usa API de Winsock2)
- 📦 **OpenSSL** instalado y accesible para el compilador

Ejemplo de instalación de OpenSSL en Windows con **vcpkg**:
```bash
vcpkg install openssl:x64-windows
```

---

## ⚙️ Compilación
Con **g++** (MinGW) y OpenSSL:
```bash
g++ -std=c++17 main.cpp Client.cpp Server.cpp NetworkHelper.cpp CryptoHelper.cpp -lws2_32 -lssl -lcrypto -o E2EE.exe
```

Con **Visual Studio**:
1. 📂 Crear un nuevo proyecto de consola.
2. 📄 Agregar todos los `.cpp` y `.h`.
3. 🔗 Configurar inclusión y enlace de OpenSSL.
4. ▶️ Compilar en **x64**.

---

## ▶️ Uso
**Servidor**:
```bash
E2EE.exe server <puerto>
```
Ejemplo:
```bash
E2EE.exe server 12345
```

**Cliente**:
```bash
E2EE.exe client <ip_servidor> <puerto>
```
Ejemplo:
```bash
E2EE.exe client 127.0.0.1 12345
```

---

## 🔄 Flujo de Comunicación
1. 🖥 **Servidor** inicia y espera conexión.
2. 💻 **Cliente** conecta al servidor.
3. 🔑 Intercambio de claves públicas RSA.
4. 📦 Cliente genera clave AES y la envía cifrada al servidor.
5. 💬 Ambos inician chat cifrado con AES.

---

## 📜 Ejemplo de Sesión
**Servidor**:
```
[Server] Iniciando servidor en el puerto 12345...
[Server] Esperando conexión de un cliente...
[Server] Cliente conectado.
[Server] Clave AES intercambiada exitosamente.
[Cliente]: Hola
Servidor:
```

**Cliente**:
```
[Client] Conectando al servidor 127.0.0.1:12345...
[Client] Conexión establecida.
[Client] Clave pública del servidor recibida.
[Client] Clave pública del cliente enviada.
[Client] Clave AES cifrada y enviada al servidor.
Cliente: Hola
[Servidor]: Hola
Cliente:
```

---

## 📄 Licencia
Este proyecto es de uso académico y está licenciado bajo **MIT**.

---

## 👨‍💻 Créditos
- 📚 **Librerías utilizadas:**
  - **OpenSSL** (RSA/AES)
  - **Winsock2** (TCP en Windows)
