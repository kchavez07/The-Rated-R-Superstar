/**
 * @file main.cpp
 * @brief Punto de entrada para la aplicación de chat cifrado RSA/AES.
 *
 * @details
 * Permite iniciar la aplicación en modo servidor o cliente:
 *  - **Servidor**:
 *    - Inicia un servidor TCP en el puerto especificado.
 *    - Acepta un cliente, intercambia claves RSA y recibe la clave AES cifrada.
 *    - Inicia el bucle de chat con envío y recepción simultánea.
 *  - **Cliente**:
 *    - Conecta al servidor en la IP y puerto indicados.
 *    - Intercambia claves RSA y envía la clave AES cifrada.
 *    - Inicia el bucle de chat con envío y recepción simultánea.
 *
 * @note Usa las clases Server y Client para manejar la lógica de red y cifrado.
 */

#include "Prerequisites.h"
#include "Server.h"
#include "Client.h"
static void runServer(int port) {
  Server s(port);
  if (!s.Start()) {
    std::cerr << "[Main] No se pudo iniciar el servidor.\n";
    return;
  }
  s.WaitForClient(); // Intercambio de claves
  s.StartChatLoop(); // Ahora recibe y envía en paralelo
}

static void runClient(const std::string& ip, int port) {
  Client c(ip, port);
  if (!c.Connect()) { std::cerr << "[Main] No se pudo conectar.\n"; return; }

  c.ExchangeKeys();
  c.SendAESKeyEncrypted();

  // ahora sí, chat en paralelo:
  c.StartChatLoop();
}

int main(int argc, char** argv) {
  std::string mode, ip;
  int port = 0;

  if (argc >= 2) {
    mode = argv[1];
    if (mode == "server") {
      port = (argc >= 3) ? std::stoi(argv[2]) : 12345;
    }
    else if (mode == "client") {
      if (argc < 4) { std::cerr << "Uso: E2EE client <ip> <port>\n"; return 1; }
      ip = argv[2];
      port = std::stoi(argv[3]);
    }
    else {
      std::cerr << "Modo no reconocido. Usa: server | client\n";
      return 1;
    }
  }
  else {
    std::cout << "Modo (server/client): ";
    std::cin >> mode;
    if (mode == "server") {
      std::cout << "Puerto: ";
      std::cin >> port;
    }
    else if (mode == "client") {
      std::cout << "IP: ";
      std::cin >> ip;
      std::cout << "Puerto: ";
      std::cin >> port;
    }
    else {
      std::cerr << "Modo no reconocido.\n";
      return 1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  if (mode == "server") runServer(port);
  else runClient(ip, port);

  return 0;
}
