/**
 * @file Client.h
 * @brief Cliente TCP con handshake RSA y mensajería cifrada con AES.
 *
 * @details
 * Esta clase encapsula la lógica de un cliente que:
 *  - Establece conexión TCP con un servidor.
 *  - Intercambia claves públicas (RSA) durante el arranque.
 *  - Envía la clave de sesión AES cifrada con la RSA del servidor.
 *  - Transmite y recibe mensajes usando cifrado simétrico (AES).
 *  - Ofrece bucles de envío/recepción para chat simple.
 *
 * @note Codificación sugerida: UTF-8 (sin BOM) para evitar problemas de caracteres en consola.
 */

#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

 /**
  * @class Client
  * @brief Cliente que se conecta por TCP, negocia claves y envía/recibe mensajes cifrados.
  *
  * @par Flujo típico de uso
  * 1. Construir `Client(ip, port)`.
  * 2. `Connect()` para establecer la conexión TCP.
  * 3. `ExchangeKeys()` para intercambio de claves (recibir RSA del servidor y preparar AES).
  * 4. `SendAESKeyEncrypted()` para enviar la clave/IV AES cifrados con RSA del servidor.
  * 5. `StartReceiveLoop()` y/o `StartChatLoop()`/`SendEncryptedMessageLoop()` para chatear.
  * 6. Destruir el objeto para liberar recursos/sockets.
  *
  * @warning Las operaciones de red pueden ser bloqueantes si no se configuran timeouts
  *          (ver implementación de NetworkHelper).
  */
class Client {
public:
	/// @brief Constructor por defecto (no conecta ni inicializa sockets).
	Client() = default;

	/**
	 * @brief Construye el cliente con destino y puerto.
	 * @param ip Dirección del servidor (por ejemplo, "127.0.0.1").
	 * @param port Puerto TCP del servidor (por ejemplo, 5000).
	 *
	 * @post El socket aún no está conectado; se debe invocar @ref Connect().
	 */
	Client(const std::string& ip, int port);

	/// @brief Destructor: libera recursos y cierra socket si está abierto.
	~Client();

	/**
	 * @brief Establece conexión con el servidor.
	 * @return true si la conexión fue exitosa.
	 * @return false si ocurrió un error.
	 *
	 * @post Si retorna true, el socket queda listo para el intercambio de claves.
	 * @note Puede registrar mensajes de diagnóstico a la consola.
	 */
	bool Connect();

	/**
	 * @brief Intercambia claves públicas con el servidor (handshake RSA).
	 *
	 * @details
	 * Secuencia esperada:
	 *  - Recibir la clave pública RSA del servidor.
	 *  - Opcionalmente enviar la clave pública del cliente (si se usa RSA bilateral).
	 *  - Preparar material de sesión (clave/IV) para AES.
	 *
	 * @pre Conexión TCP establecida mediante @ref Connect().
	 * @post Tras completarse, el cliente está listo para @ref SendAESKeyEncrypted().
	 */
	void ExchangeKeys();

	/**
	 * @brief Cifra la clave AES con la pública del servidor y la envía.
	 *
	 * @details
	 * Serializa y envía el material de sesión (p.ej., clave AES e IV) protegido
	 * con RSA (clave pública del servidor). El servidor podrá descifrarlo con su
	 * clave privada y así establecer el canal simétrico.
	 *
	 * @pre Debe haberse ejecutado @ref ExchangeKeys().
	 */
	void SendAESKeyEncrypted();

	/**
	 * @brief Cifra un mensaje con AES y lo envía al servidor.
	 * @param message Texto plano a cifrar y enviar.
	 *
	 * @pre La sesión AES debe estar establecida (ver @ref SendAESKeyEncrypted()).
	 * @note Usa @ref NetworkHelper para el envío con framing y @ref CryptoHelper para el cifrado.
	 */
	void SendEncryptedMessage(const std::string& message);

	/**
	 * @brief Bucle interactivo de envío de mensajes cifrados (lado cliente).
	 *
	 * @details
	 * Lee entradas del usuario (consola), las cifra con AES y las envía al servidor
	 * hasta que se indique finalizar (p.ej. EOF o comando de salida).
	 *
	 * @warning Bloquea el hilo actual mientras está activo.
	 */
	void SendEncryptedMessageLoop();

	/**
	 * @brief Bucle de chat de alto nivel (envío/recepción) según la implementación.
	 *
	 * @details
	 * Dependiendo de la implementación, puede combinar lectura de consola,
	 * envío cifrado y recepción/impresión de respuestas del servidor.
	 *
	 * @note Si también se usa @ref StartReceiveLoop(), coordinar hilos/vida del socket.
	 */
	void StartChatLoop();

	/**
	 * @brief Bucle de recepción: recibe mensajes del servidor y los muestra.
	 *
	 * @details
	 * Extrae frames de red, descifra el contenido con AES y los imprime en consola.
	 * Finaliza si el socket se cierra o ocurre un error de red.
	 *
	 * @warning Puede ser bloqueante. Ejecutarlo idealmente en un hilo dedicado.
	 */
	void StartReceiveLoop();     // Recibir y mostrar mensajes del servidor

private:
	/** @brief Dirección IP o hostname del servidor de destino. */
	std::string m_ip;

	/** @brief Puerto TCP de conexión. */
	int m_port;

	/** @brief Socket conectado al servidor (válido tras @ref Connect()). */
	SOCKET m_serverSock;

	/** @brief Utilidades de red (framing, timeouts, envío/recepción). */
	NetworkHelper m_net;

	/** @brief Utilidades criptográficas (RSA/AES). */
	CryptoHelper m_crypto;
};
