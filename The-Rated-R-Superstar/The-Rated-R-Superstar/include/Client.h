/**
 * @file Client.h
 * @brief Cliente TCP con handshake RSA y mensajer�a cifrada con AES.
 *
 * @details
 * Esta clase encapsula la l�gica de un cliente que:
 *  - Establece conexi�n TCP con un servidor.
 *  - Intercambia claves p�blicas (RSA) durante el arranque.
 *  - Env�a la clave de sesi�n AES cifrada con la RSA del servidor.
 *  - Transmite y recibe mensajes usando cifrado sim�trico (AES).
 *  - Ofrece bucles de env�o/recepci�n para chat simple.
 *
 * @note Codificaci�n sugerida: UTF-8 (sin BOM) para evitar problemas de caracteres en consola.
 */

#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

 /**
  * @class Client
  * @brief Cliente que se conecta por TCP, negocia claves y env�a/recibe mensajes cifrados.
  *
  * @par Flujo t�pico de uso
  * 1. Construir `Client(ip, port)`.
  * 2. `Connect()` para establecer la conexi�n TCP.
  * 3. `ExchangeKeys()` para intercambio de claves (recibir RSA del servidor y preparar AES).
  * 4. `SendAESKeyEncrypted()` para enviar la clave/IV AES cifrados con RSA del servidor.
  * 5. `StartReceiveLoop()` y/o `StartChatLoop()`/`SendEncryptedMessageLoop()` para chatear.
  * 6. Destruir el objeto para liberar recursos/sockets.
  *
  * @warning Las operaciones de red pueden ser bloqueantes si no se configuran timeouts
  *          (ver implementaci�n de NetworkHelper).
  */
class Client {
public:
	/// @brief Constructor por defecto (no conecta ni inicializa sockets).
	Client() = default;

	/**
	 * @brief Construye el cliente con destino y puerto.
	 * @param ip Direcci�n del servidor (por ejemplo, "127.0.0.1").
	 * @param port Puerto TCP del servidor (por ejemplo, 5000).
	 *
	 * @post El socket a�n no est� conectado; se debe invocar @ref Connect().
	 */
	Client(const std::string& ip, int port);

	/// @brief Destructor: libera recursos y cierra socket si est� abierto.
	~Client();

	/**
	 * @brief Establece conexi�n con el servidor.
	 * @return true si la conexi�n fue exitosa.
	 * @return false si ocurri� un error.
	 *
	 * @post Si retorna true, el socket queda listo para el intercambio de claves.
	 * @note Puede registrar mensajes de diagn�stico a la consola.
	 */
	bool Connect();

	/**
	 * @brief Intercambia claves p�blicas con el servidor (handshake RSA).
	 *
	 * @details
	 * Secuencia esperada:
	 *  - Recibir la clave p�blica RSA del servidor.
	 *  - Opcionalmente enviar la clave p�blica del cliente (si se usa RSA bilateral).
	 *  - Preparar material de sesi�n (clave/IV) para AES.
	 *
	 * @pre Conexi�n TCP establecida mediante @ref Connect().
	 * @post Tras completarse, el cliente est� listo para @ref SendAESKeyEncrypted().
	 */
	void ExchangeKeys();

	/**
	 * @brief Cifra la clave AES con la p�blica del servidor y la env�a.
	 *
	 * @details
	 * Serializa y env�a el material de sesi�n (p.ej., clave AES e IV) protegido
	 * con RSA (clave p�blica del servidor). El servidor podr� descifrarlo con su
	 * clave privada y as� establecer el canal sim�trico.
	 *
	 * @pre Debe haberse ejecutado @ref ExchangeKeys().
	 */
	void SendAESKeyEncrypted();

	/**
	 * @brief Cifra un mensaje con AES y lo env�a al servidor.
	 * @param message Texto plano a cifrar y enviar.
	 *
	 * @pre La sesi�n AES debe estar establecida (ver @ref SendAESKeyEncrypted()).
	 * @note Usa @ref NetworkHelper para el env�o con framing y @ref CryptoHelper para el cifrado.
	 */
	void SendEncryptedMessage(const std::string& message);

	/**
	 * @brief Bucle interactivo de env�o de mensajes cifrados (lado cliente).
	 *
	 * @details
	 * Lee entradas del usuario (consola), las cifra con AES y las env�a al servidor
	 * hasta que se indique finalizar (p.ej. EOF o comando de salida).
	 *
	 * @warning Bloquea el hilo actual mientras est� activo.
	 */
	void SendEncryptedMessageLoop();

	/**
	 * @brief Bucle de chat de alto nivel (env�o/recepci�n) seg�n la implementaci�n.
	 *
	 * @details
	 * Dependiendo de la implementaci�n, puede combinar lectura de consola,
	 * env�o cifrado y recepci�n/impresi�n de respuestas del servidor.
	 *
	 * @note Si tambi�n se usa @ref StartReceiveLoop(), coordinar hilos/vida del socket.
	 */
	void StartChatLoop();

	/**
	 * @brief Bucle de recepci�n: recibe mensajes del servidor y los muestra.
	 *
	 * @details
	 * Extrae frames de red, descifra el contenido con AES y los imprime en consola.
	 * Finaliza si el socket se cierra o ocurre un error de red.
	 *
	 * @warning Puede ser bloqueante. Ejecutarlo idealmente en un hilo dedicado.
	 */
	void StartReceiveLoop();     // Recibir y mostrar mensajes del servidor

private:
	/** @brief Direcci�n IP o hostname del servidor de destino. */
	std::string m_ip;

	/** @brief Puerto TCP de conexi�n. */
	int m_port;

	/** @brief Socket conectado al servidor (v�lido tras @ref Connect()). */
	SOCKET m_serverSock;

	/** @brief Utilidades de red (framing, timeouts, env�o/recepci�n). */
	NetworkHelper m_net;

	/** @brief Utilidades criptogr�ficas (RSA/AES). */
	CryptoHelper m_crypto;
};
