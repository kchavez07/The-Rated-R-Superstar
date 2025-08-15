/**
 * @file CryptoHelper.h
 * @brief Utilidades criptogr�ficas para generaci�n de claves RSA y cifrado/descifrado AES.
 *
 * @details
 * Esta clase encapsula las operaciones de cifrado necesarias para el sistema Cliente-Servidor:
 *  - Generaci�n de claves RSA (2048 bits) y exportaci�n/importaci�n en formato PEM.
 *  - Generaci�n de clave AES-256 aleatoria.
 *  - Cifrado y descifrado de la clave AES usando RSA.
 *  - Cifrado y descifrado de mensajes con AES-256 en modo CBC.
 *
 * @note La implementaci�n se basa en OpenSSL.
 * @warning La clase administra memoria de claves RSA (punteros `RSA*`), por lo que el destructor debe liberar correctamente los recursos.
 */

#pragma once
#include "Prerequisites.h"
#include "openssl\rsa.h"
#include "openssl\aes.h"

 /**
  * @class CryptoHelper
  * @brief Proporciona funciones para el manejo de claves y cifrado RSA/AES.
  *
  * @par Funcionalidades principales:
  *  - **RSA**:
  *    - Generar par de claves (privada/p�blica).
  *    - Exportar clave p�blica a string PEM.
  *    - Cargar clave p�blica de un peer desde string PEM.
  *    - Cifrar y descifrar la clave AES de sesi�n usando RSA.
  *  - **AES**:
  *    - Generar clave AES-256 aleatoria (32 bytes).
  *    - Cifrar y descifrar mensajes con AES-256-CBC.
  *
  * @note Todas las funciones asumen codificaci�n UTF-8 para strings.
  */
class CryptoHelper {
public:
    /// @brief Constructor: inicializa punteros de clave en nullptr.
    CryptoHelper();

    /// @brief Destructor: libera memoria de claves RSA asignadas.
    ~CryptoHelper();

    //   RSA
    /**
     * @brief Genera un nuevo par de claves RSA de 2048 bits.
     *
     * @post La clave privada y la clave p�blica quedan almacenadas en @ref rsaKeyPair.
     * @throws std::runtime_error si la generaci�n falla.
     */
    void GenerateRSAKeys();

    /**
     * @brief Devuelve la clave p�blica en formato PEM.
     * @return Clave p�blica como string codificado en PEM.
     * @pre Debe haberse generado el par de claves con @ref GenerateRSAKeys().
     */
    std::string GetPublicKeyString() const;

    /**
     * @brief Carga la clave p�blica del peer desde un string PEM.
     * @param pemKey Clave p�blica codificada en formato PEM.
     * @throws std::runtime_error si el parseo del PEM falla.
     */
    void LoadPeerPublicKey(const std::string& pemKey);

    //   AES
    /**
     * @brief Genera una clave AES-256 (32 bytes aleatorios).
     * @post La clave queda almacenada en @ref aesKey.
     */
    void GenerateAESKey();

    /**
     * @brief Cifra la clave AES con la clave p�blica del peer usando RSA.
     * @return Clave AES cifrada como vector de bytes.
     * @pre Debe haberse cargado la clave p�blica del peer con @ref LoadPeerPublicKey().
     */
    std::vector<unsigned char> EncryptAESKeyWithPeer();

    /**
     * @brief Descifra la clave AES enviada por el cliente.
     * @param encryptedKey Vector con la clave AES cifrada.
     * @pre Debe haberse generado el par de claves RSA del servidor con @ref GenerateRSAKeys().
     * @post La clave AES descifrada se almacena en @ref aesKey.
     */
    void DecryptAESKey(const std::vector<unsigned char>& encryptedKey);

    /**
     * @brief Cifra un mensaje usando AES-256 en modo CBC.
     * @param plaintext Texto plano a cifrar.
     * @param outIV Vector donde se guardar� el IV usado para el cifrado.
     * @return El texto cifrado como vector de bytes.
     * @pre La clave AES debe estar generada o cargada en @ref aesKey.
     * @note El IV es aleatorio en cada cifrado y se devuelve en @p outIV.
     */
    std::vector<unsigned char> AESEncrypt(const std::string& plaintext, std::vector<unsigned char>& outIV);

    /**
     * @brief Descifra un mensaje cifrado con AES-256-CBC.
     * @param ciphertext Vector con el texto cifrado.
     * @param iv Vector con el IV usado durante el cifrado.
     * @return Texto plano original.
     * @pre La clave AES debe estar cargada en @ref aesKey.
     */
    std::string AESDecrypt(const std::vector<unsigned char>& ciphertext,
        const std::vector<unsigned char>& iv);

private:
    RSA* rsaKeyPair;             ///< Par de claves RSA propio (privada/p�blica).
    RSA* peerPublicKey;          ///< Clave p�blica RSA del peer (remoto).
    unsigned char aesKey[32];    ///< Clave AES-256 (32 bytes).
};
