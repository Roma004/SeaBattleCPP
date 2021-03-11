#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <string>


void sendData(sf::TcpSocket &socket, std::string data) {
    sf::Packet packet;
    packet << data;
    socket.send(packet);
}

std::string receiveData(sf::TcpSocket &socket) {
    std::string data;
    sf::Packet packet;
    socket.receive(packet);

    packet >> data;

    return data;
}