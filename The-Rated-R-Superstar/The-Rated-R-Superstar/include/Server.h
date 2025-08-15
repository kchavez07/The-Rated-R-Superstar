/**
 * @file Server.h
 * @brief Servidor TCP con handshake RSA y comunicaci�n cifrada AES.
 *
 * @details
 * Esta clase implementa un servidor TCP que:
 *  - Escucha conexiones entrantes en un puerto espec�fico.
 *  - Acepta un cliente y realiza el intercambio de claves p�blicas (RSA).
 *  - Establece una clave de sesi�n AES cifrada con RSA.
 *  - Recibe y env�a mensajes cifrados con AES.
 *  - Ofrece bucles de env�o y recepci�n para implementar un chat simple.
 *
 * @note Utiliza @ref NetworkHelper para la comunicaci�n y @ref CryptoHelper para las operaciones criptogr�ficas.
 */

#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

 /**
  * @class Server
  * @brief Servidor TCP que negocia claves RSA/AES y mantiene comunicaci�n cifrada.
  *
  * @par Flujo t�pico de uso:
  *  1. Construir `Server(port)`.
  *  2. `Start()` para iniciar la escucha en el puerto.
  *  3. `WaitForClient()` para aceptar un cliente y hacer el handshake RSA.
  *  4. `StartReceiveLoop()` y/o `SendEncryptedMessageLoop()` para intercambiar mensajes.
  *  5. Finalizar con cierre limpio de sockets y hilos.
  *
  * @warning Las funciones de bucle son bloqueantes y deber�an ejecutarse en hilos separados si se requiere env�o y recepci�n simult�nea.
  */
class Server {
public:
    /// @brief Constructor por defecto (no inicia el servidor).
    Server() = default;

    /**
     * @brief Construye el servidor con un puerto de escucha.
     * @param port Puerto TCP en el que se escuchar�n conexiones entrantes.
     */
    Server(int port);

    /// @brief Destructor: libera recursos, cierra sockets y detiene hilos.
    ~Server();

    /**
     * @brief Inicia el servidor en el puerto especificado.
     * @return true si el servidor se inicializ� correctamente.
     * @return false si hubo un error en la configuraci�n.
     * @post El servidor queda a la espera de clientes hasta llamar a @ref WaitForClient().
     */
    bool Start();

    /**
     * @brief Espera a que un cliente se conecte e intercambia claves p�blicas.
     *
     * @details
     * Secuencia:
     *  - Aceptar un cliente entrante.
     *  - Enviar clave p�blica RSA del servidor.
     *  - Recibir clave AES cifrada con la RSA del servidor.
     * @pre El servidor debe estar iniciado con @ref Start().
     */
    void WaitForClient();

    /**
     * @brief Recibe un mensaje cifrado del cliente, lo descifra y lo imprime.
     *
     * @pre La sesi�n AES debe estar establecida tras @ref WaitForClient().
     * @note La impresi�n es directa en consola; para uso en GUI, procesar el texto devuelto.
     */
    void ReceiveEncryptedMessage();

    /**
     * @brief Bucle de recepci�n continua de mensajes cifrados.
     *
     * @details
     * Ejecuta lectura desde el socket del cliente, descifra con AES y
     * muestra el resultado hasta que se detenga la bandera @ref m_running o el socket se cierre.
     * @warning Bloquea el hilo actual mientras est� activo.
     */
    void StartReceiveLoop();

    /**
     * @brief Bucle de env�o de mensajes cifrados al cliente.
     *
     * @details
     * Lee entradas desde consola, las cifra con AES y las env�a al cliente hasta
     * que se detenga la comunicaci�n.
     */
    void SendEncryptedMessageLoop();

    /**
     * @brief Bucle de chat combinado (env�o y recepci�n) seg�n implementaci�n.
     *
     * @details
     * Puede crear hilos internos para recibir y enviar simult�neamente, usando
     * @ref StartReceiveLoop() y @ref SendEncryptedMessageLoop().
     */
    void StartChatLoop();

private:
    int m_port;                       ///< Puerto TCP en el que escucha el servidor.
    SOCKET m_clientSock;               ///< Socket del cliente conectado.
    NetworkHelper m_net;               ///< Utilidad de red para env�o/recepci�n.
    CryptoHelper m_crypto;             ///< Utilidad criptogr�fica para RSA/AES.
    std::thread m_rxThread;            ///< Hilo de recepci�n de mensajes.
    std::atomic<bool> m_running{ false };///< Bandera de control para bucles activos.
};
