# Regularização Alimentar - Sistema IoT

## Descrição do Projeto
Sistema de lembretes inteligente para horários de refeições utilizando ESP32 e protocolo MQTT. Desenvolvido como trabalho acadêmico na Universidade Presbiteriana Mackenzie.

## Objetivo
Promover regularidade alimentar através de alertas sonoros e visuais, prevenindo problemas de saúde causados por hábitos alimentares irregulares.

## Hardware Utilizado
- **Microcontrolador:** ESP32
- **Sensor:** Switch/Interruptor (substituiu o sensor RTC original)
- **Atuadores:** 
  - Buzzer piezoelétrico (alerta sonoro)
  - LED vermelho (alerta visual)
  - Display OLED 0.96" I2C (interface)
- **Conexões:** Resistores 220Ω

## Esquema de Ligação

| Componente | Pino ESP32 | Função |
|------------|------------|---------|
| Buzzer | GPIO 25 | Alarme sonoro |
| LED | GPIO 26 | Indicador visual |
| Switch | GPIO 4 | Sensor de confirmação |
| OLED SDA | GPIO 21 | Comunicação I2C |
| OLED SCL | GPIO 22 | Comunicação I2C |

## Protocolo de Comunicação
- **Broker MQTT:** HiveMQ Cloud (broker.hivemq.com)
- **Porta:** 1883 (MQTT) / 8884 (WebSocket)
- **Tópicos:**
  - `regularizacao_alimentar/alerta` (recebe comandos)
  - `regularizacao_alimentar/confirmacao` (envia confirmações)
  - `regularizacao_alimentar/sensor` (envia estados do sensor)

## Funcionamento
1. **Inicialização:** ESP32 conecta ao WiFi e broker MQTT
2. **Ativação:** Comando "INICIAR" via MQTT aciona buzzer e LED
3. **Confirmação:** Usuário move o switch para parar alarme
4. **Registro:** Sistema publica confirmação e mede tempos de resposta

## Resultados Obtidos

### Medições de Tempo de Resposta

| Componente | Tempo Médio |
|------------|-------------|
| Sensor (Switch) | 3ms |
| Atuador (Buzzer) | 36.25ms |

## Como Utilizar
1. Carregue o código no ESP32 (Wokwi ou Arduino IDE)
2. Conecte-se ao WiFi "Wokwi-GUEST"
3. Inscreva-se nos tópicos MQTT no HiveMQ Web Client
4. Publique "INICIAR" no tópico de alerta para testar

## Estrutura do Projeto
regularizacao-alimentar-iot/
├── src/ # Código fonte
├── docs/ # Documentação
├── diagramas/ # Esquemas e fluxogramas
├── img/ # Imagens do projeto
└── README.md # Este arquivo

## Demonstração
https://www.youtube.com/watch?v=VuffliUhWhU

## Autores
- **Rafael Oliveira Viana** - 10416106@mackenzista.com.br

## Instituição
Faculdade de Computação e Informática  
Universidade Presbiteriana Mackenzie (UPM)  
São Paulo, SP - Brazil

## Links Úteis
- Simulador Wokwi: https://wokwi.com
- HiveMQ Web Client: http://www.hivemq.com/demos/websocket-client/

---

*OBJETOS INTELIGENTES CONECTADOS - 2025*
