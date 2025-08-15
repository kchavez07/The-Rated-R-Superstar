/**
 * @file NetworkHelper.h
 * @brief Utilidad para manejo de sockets TCP (modo servidor y cliente).
 *
 * @details
 * Esta clase encapsula operaciones de red sobre Winsock2 para simplificar:
 *  - Creación y configuración de sockets.
 *  - Inicio de un servidor TCP y aceptación de clientes.
 *  - Conexión a un servidor TCP remoto.
 *  - Envío y recepción de datos en formato texto y binario.
 *  - Envío/recepción garantizando tamaño exacto.
 *  - Cierre seguro de sockets.
 *
 * @note Requiere la inicialización de Winsock (`WSAStartup`) previa en la aplicación.
 * @warning Las funciones son bloqueantes a menos que el socket esté configurado como no bloqueante.
 */

#pragma once
#include "Prerequisites.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

 /**
  * @class NetworkHelper
  * @brief Abstracción para operaciones de red TCP en cliente y servidor.
  *
  * @par Funcionalidades principales:
  *  - **Servidor**:
  *    - Iniciar socket en modo escucha (`StartServer`).
  *    - Aceptar cliente (`AcceptClient`).
  *  - **Cliente**:
  *    - Conectar a servidor (`ConnectToServer`).
  *  - **Comunicación**:
  *    - Enviar datos como texto o binario.
  *    - Recibir datos como texto o binario.
  *    - Funciones para envío y recepción exacta de N bytes.
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
     * @param port Puerto TCP que se usará para escuchar conexiones entrantes.
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
     * @param ip Dirección IP del servidor.
     * @param port Puerto del servidor.
     * @return true Si la conexión fue exitosa.
     * @return false Si falló la conexión.
     */
    bool ConnectToServer(const std::string& ip, int port);

    //   Envío y recepción
    /**
     * @brief Envía una cadena de texto por el socket.
     * @param socket Descriptor de socket válido.
     * @param data Texto a enviar.
     * @return true si se enviaron todos los datos, false si hubo error.
     */
    bool SendData(SOCKET socket, const std::string& data);

    /**
     * @brief Envía datos binarios (ej. claves AES, RSA) por el socket.
     * @param socket Descriptor de socket válido.
     * @param data Datos binarios a enviar.
     * @return true si se enviaron todos los datos, false en error.
     */
    bool SendData(SOCKET socket, const std::vector<unsigned char>& data);

    /**
     * @brief Recibe una cadena de texto del socket.
     * @param socket Descriptor de socket válido.
     * @return Cadena recibida; puede estar vacía si el socket se cerró o hubo error.
     */
    std::string ReceiveData(SOCKET socket);

    /**
     * @brief Recibe datos binarios del socket.
     * @param socket Descriptor de socket válido.
     * @param size Número exacto de bytes a recibir (0 para recibir hasta cierre).
     * @return Vector con los bytes recibidos.
     */
    std::vector<unsigned char> ReceiveDataBinary(SOCKET socket, int size = 0);

    /**
     * @brief Cierra un socket de forma segura.
     * @param socket Descriptor de socket a cerrar.
     */
    void close(SOCKET socket);

    /**
     * @brief Envía todos los bytes de un buffer.
     * @param s Socket válido.
     * @param data Puntero al buffer.
     * @param len Número de bytes a enviar.
     * @return true si se enviaron todos; false si hubo error.
     * @note Útil para asegurar que no se pierda parte del mensaje.
     */
    bool SendAll(SOCKET s, const unsigned char* data, int len);

    /**
     * @brief Recibe exactamente len bytes del socket.
     * @param s Socket válido.
     * @param out Puntero al buffer de destino.
     * @param len Número de bytes a recibir.
     * @return true si se recibieron todos; false si hubo error o cierre.
     */
    bool ReceiveExact(SOCKET s, unsigned char* out, int len);

public:
    SOCKET m_serverSocket = -1;  ///< Socket del servidor (modo escucha).
private:
    bool m_initialized;          ///< Indica si Winsock fue inicializado correctamente.
};
