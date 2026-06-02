# Monitoramento de Som com NodeMCU e MQTT
Descrição

Projeto desenvolvido para monitoramento de níveis sonoros em tempo real utilizando comunicação MQTT e visualização em dashboard web.

Tecnologias Utilizadas
NodeMCU ESP8266
Sensor de Som MAX4466
Protocolo MQTT
HiveMQ Cloud
Node-RED Dashboard
Arquitetura do Sistema

MAX4466 → NodeMCU ESP8266 → HiveMQ Cloud → Node-RED → Dashboard Web

Funcionamento

O sensor MAX4466 capta as variações de pressão sonora do ambiente e envia o sinal analógico para o NodeMCU através da porta A0.

O NodeMCU realiza leituras contínuas do sinal, calcula a amplitude sonora por meio da diferença entre os valores máximos e mínimos obtidos em uma janela de amostragem de 100 ms e gera um indicador de intensidade sonora.

A cada segundo, o valor calculado é publicado no broker MQTT HiveMQ Cloud utilizando o tópico:

sensor/som

O Node-RED atua como cliente MQTT, recebendo os dados publicados pelo NodeMCU. Os valores são processados e exibidos em tempo real por meio de um Dashboard Web contendo um medidor (Gauge) para acompanhamento da intensidade sonora.

Exemplo de Mensagem MQTT

Payload:
125
Tópico:
sensor/som

Aplicações
Monitoramento de ruído ambiental
Automação residencial
Monitoramento de ambientes educacionais
Projetos de Internet das Coisas (IoT)
Sistemas de alerta sonoro
Resultados

O sistema permite visualizar em tempo real a intensidade sonora captada pelo sensor, demonstrando a integração entre dispositivos IoT, comunicação MQTT em nuvem e ferramentas de monitoramento baseadas em Node-RED.

# Link para vídeo demonstração:
<https://youtu.be/r-oVzcYupNI>
