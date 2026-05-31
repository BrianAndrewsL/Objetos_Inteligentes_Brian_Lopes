# Monitoramento de Ruído com NodeMCU e MQTT

Projeto desenvolvido utilizando:

- NodeMCU ESP8266
- Microfone MAX4466
- MQTT
- Mosquitto
- Node-RED

## Arquitetura

MAX4466 -> NodeMCU -> MQTT Broker -> Node-RED

## Funcionamento

O NodeMCU realiza a leitura do sensor de áudio e publica os valores no tópico MQTT `sensor/ruido`.
O Node-RED recebe os dados e exibe as informações em tempo real.
