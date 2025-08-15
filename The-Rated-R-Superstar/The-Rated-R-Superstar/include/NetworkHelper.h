/**
 * @file NetworkHelper.h
 * @brief Utilidad para manejo de sockets TCP (modo servidor y cliente).
 *
 * @details
 * Esta clase encapsula operaciones de red sobre Winsock2 para simplificar:
 *  - Creaci�n y configuraci�n de sockets.
 *  - Inicio de un servidor TCP y aceptaci�n de clientes.
 *  - Conexi�n a un servidor TCP remoto.
 *  - Env�o y recepci�n de datos en formato texto y binario.
 *  - Env�o/recepci�n garantizando tama�o exacto.
 *  - Cierre seguro de sockets.
 *
 * @note Requiere la inicializaci�n de Winsock (`WSAStartup`) previa en la aplicaci�n.
 * @warning Las funciones son bloqueantes a menos que el socket est� configurado como no bloqueante.
 */

#pragma once
#include "Prerequisites.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

 /**
  * @class NetworkHelper
  * @brief Abstracci�n para operaciones de red TCP en cliente y servidor.
  *
  * @par Funcionalidades principales:
  *  - **Servidor**:
  *    - Iniciar socket en modo escucha (`StartServer`).
  *    - Aceptar cliente (`AcceptClient`).
  *  - **Cliente**:
  *    - Conectar a servidor (`ConnectToServer`).
  *  - **Comunicaci�n**:
  *    - Enviar datos como texto o binario.
  *    - Recibir datos como texto o binario.
  *    - Funciones para env�o y recepci�n exacta de N bytes.
  */
class NetworkHelper {
public:
    /// @brief Constructor: inicializa estado interno.
    NetworkHelper();

    /// @brief Destructor: libera recursos si es necesario.
    ~NetworkHelper();

    //   Servidor
    /**
     * @brief Inicia un socket servidor en el puerto indicado y lo deja en modo escucha.
     * @param port Puerto TCP que se usar� para escuchar conexiones entrantes.
     * @return true Si el servidor se inicia correctamente.
     * @return false Si ocurre un error en cualquier paso.
     * @post El socket de escucha se almacena en @ref m_serverSocket.
     */
    bool StartServer(int port);

    /**
     * @brief Espera y acepta un cliente entrante.
     * @return SOCKET del cliente aceptado, o INVALID_SOCKET si falla.
     * @pre El servidor debe estar en modo escucha tras @ref StartServer().
     */
    SOCKET AcceptClient();

    //   Cliente
    /**
     * @brief Conecta al servidor especificado por IP y puerto.
     * @param ip Direcci�n IP del servidor.
     * @param port Puerto del servidor.
     * @return true Si la conexi�n fue exitosa.
     * @return false Si fall� la conexi�n.
     */
    bool ConnectToServer(const std::string& ip, int port);

    //   Env�o y recepci�n
    /**
     * @brief Env�a una cadena de texto por el socket.
     * @param socket Descriptor de socket v�lido.
     * @param data Texto a enviar.
     * @return true si se enviaron todos los datos, false si hubo error.
     */
    bool SendData(SOCKET socket, const std::string& data);

    /**
     * @brief Env�a datos binarios (ej. claves AES, RSA) por el socket.
     * @param socket Descriptor de socket v�lido.
     * @param data Datos binarios a enviar.
     * @return true si se enviaron todos los datos, false en error.
     */
    bool SendData(SOCKET socket, const std::vector<unsigned char>& data);

    /**
     * @brief Recibe una cadena de texto del socket.
     * @param socket Descriptor de socket v�lido.
     * @return Cadena recibida; puede estar vac�a si el socket se cerr� o hubo error.
     */
    std::string ReceiveData(SOCKET socket);

    /**
     * @brief Recibe datos binarios del socket.
     * @param socket Descriptor de socket v�lido.
     * @param size N�mero exacto de bytes a recibir (0 para recibir hasta cierre).
     * @return Vector con los bytes recibidos.
     */
    std::vector<unsigned char> ReceiveDataBinary(SOCKET socket, int size = 0);

    /**
     * @brief Cierra un socket de forma segura.
     * @param socket Descriptor de socket a cerrar.
     */
    void close(SOCKET socket);

    /**
     * @brief Env�a todos los bytes de un buffer.
     * @param s Socket v�lido.
     * @param data Puntero al buffer.
     * @param len N�mero de bytes a enviar.
     * @return true si se enviaron todos; false si hubo error.
     * @note �til para asegurar que no se pierda parte del mensaje.
     */
    bool SendAll(SOCKET s, const unsigned char* data, int len);

    /**
     * @brief Recibe exactamente len bytes del socket.
     * @param s Socket v�lido.
     * @param out Puntero al buffer de destino.
     * @param len N�mero de bytes a recibir.
     * @return true si se recibieron todos; false si hubo error o cierre.
     */
    bool ReceiveExact(SOCKET s, unsigned char* out, int len);

public:
    SOCKET m_serverSocket = -1;  ///< Socket del servidor (modo escucha).
private:
    bool m_initialized;          ///< Indica si Winsock fue inicializado correctamente.
};
