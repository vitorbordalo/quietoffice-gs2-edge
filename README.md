# QuietOffice – Monitor de Ruído no Ambiente de Trabalho (ESP32 + FIWARE)

Projeto desenvolvido para a **Global Solution – Futuro do Trabalho** da FIAP.

O **QuietOffice** é um sistema de monitoramento de ruído para ambientes de trabalho, utilizando **ESP32** em simulador (**Wokwi**) integrado ao **FIWARE Orion Context Broker**.  
O objetivo é acompanhar continuamente o nível de ruído (em dB) e gerar **alertas contextuais** quando o ambiente ultrapassa um limiar pré-definido, apoiando a saúde, o foco e a produtividade das pessoas.

---

## 1. Cenário de Uso

Em muitos escritórios e espaços compartilhados, o excesso de barulho:

- prejudica a concentração e a produtividade;
- aumenta o estresse e o cansaço mental;
- impacta a qualidade das interações e reuniões.

O **QuietOffice** monitora o ruído do ambiente e:

- classifica o nível de ruído em _aceitável_ ou _alto_;
- gera uma mensagem de alerta quando o limite é ultrapassado;
- envia os dados para o **FIWARE Orion**, permitindo histórico e integração com outros sistemas (dashboards, notificações, etc.).

No simulador, o ruído é **gerado de forma aleatória**, mas o fluxo é o mesmo que seria usado com um sensor físico de som conectado ao ESP32.

---

## 2. Arquitetura da Solução

**Visão geral:**

- **ESP32 (Wokwi)**  
  - Gera um valor de ruído simulado (em dB).  
  - Avalia se o valor ultrapassa o limite configurado (ex.: 55 dB).  
  - Monta uma mensagem contextual (normal ou alerta).  
  - Envia os atributos para o FIWARE via HTTP (NGSI v2).

- **FIWARE Orion Context Broker (VM na Azure)**  
  - Recebe atualizações de contexto do ESP32.  
  - Armazena/atualiza a entidade `WorkArea:001` com:
    - `noiseLevel` (Number)  
    - `status` (Text – `quiet` ou `noisy`)  
    - `alert` (Boolean – `true`/`false`)  
    - `alertMessage` (Text – mensagem amigável)

- **Ferramentas de Teste (cURL/Postman)**  
  - Criam a entidade inicial.  
  - Consultam os valores atualizados para evidência da integração.

**Diagrama simplificado:**

```text
[ESP32 - Wokwi]
   |
   |  HTTP (NGSI v2)
   v
[FIWARE Orion Context Broker]  <--->  [Ferramentas de consulta (cURL/Postman)]
