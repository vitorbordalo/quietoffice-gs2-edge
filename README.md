QuietOffice – Monitor de Ruído (ESP32 + FIWARE + OLED)
Global Solution – Futuro do Trabalho (FIAP)

⸻

	1.	Visão geral do projeto
O QuietOffice é um sistema de monitoramento de ruído para ambientes de trabalho, desenvolvido em simulador com ESP32 (Wokwi), display OLED SSD1306 e integração com o FIWARE Orion Context Broker (VM na Azure).
O sistema:

	•	mede (simula) o nível de ruído em dB;
	•	compara com um limite configurado (55 dB);
	•	exibe no display local o nome do projeto, ruído atual, ruído limite e mensagem;
	•	envia os dados em tempo real para o FIWARE, permitindo dashboards, alertas e histórico.

No simulador, o ruído é gerado aleatoriamente, como se fosse um sensor de som real conectado ao ESP32.

⸻

	2.	Cenário e objetivo

Ambientes de trabalho compartilhados sofrem com barulho excessivo, o que prejudica foco, saúde e produtividade.
O objetivo do QuietOffice é oferecer um monitor simples de instalar que:
	•	informe continuamente o nível de ruído do ambiente;
	•	indique claramente quando o ambiente está dentro do limite ou em alerta;
	•	permita que outras aplicações (por exemplo, painéis e notificações) usem esses dados via FIWARE.

⸻

	3.	Arquitetura da solução

Componentes principais:

a) ESP32 (Wokwi)
	•	Gera um valor de ruído simulado entre 40 e 80 dB.
	•	Compara o valor lido com o limite de 55 dB.
	•	Define o status do ambiente (quiet ou noisy) e a mensagem amigável de alerta ou normalidade.
	•	Envia os atributos para o FIWARE Orion usando HTTP (NGSI v2).

b) Display OLED SSD1306 (I2C)
	•	Exibe em tempo real:
• título do projeto (QuietOffice – Monitor de Ruído);
• ruído atual (em dB);
• ruído limite (em dB);
• mensagem: “ALERTA: Nível de ruído alto no ambiente de trabalho!” ou “Ambiente dentro do nível aceitável de ruído.”

c) FIWARE Orion Context Broker (VM Azure)
	•	Entidade utilizada: WorkArea:001
	•	Atributos atualizados pelo ESP32:
• noiseLevel (Number) – nível de ruído em dB;
• status (Text) – quiet ou noisy;
• alert (Boolean) – true ou false;
• alertMessage (Text) – mensagem em português para o usuário.

Fluxo resumido:
ESP32 (com OLED) → HTTP NGSI v2 → Orion Context Broker → Consulta por cURL/Postman → prints de tela como evidência.

⸻

	4.	Execução no simulador (Wokwi)

Passo a passo:
	1.	Acessar o projeto no Wokwi (link na seção de Links de entrega).
	2.	Confirmar que a placa é “ESP32 DevKit v1”.
	3.	Verificar o circuito do display OLED SSD1306 (I2C):
	•	VCC ligado ao 3V3 do ESP32;
	•	GND ligado ao GND do ESP32;
	•	SCL ligado ao GPIO 22 do ESP32;
	•	SDA ligado ao GPIO 21 do ESP32.
	4.	No código, já estão definidos:
	•	rede Wi-Fi de simulação: SSID “Wokwi-GUEST”, sem senha;
	•	limite de ruído: 55,0 dB;
	•	intervalo de envio ao FIWARE: 5 segundos.
	5.	Clicar em “Run” no Wokwi.
	6.	Observar:
	•	no Serial Monitor: ruído atual, ruído limite e mensagem de texto;
	•	no display OLED: mesmo conteúdo, em formato resumido para o usuário final.

⸻

	5.	Integração com FIWARE e evidências (prints)

a) Verificação do Orion
	•	É feita via requisição HTTP para o endpoint /version do Orion na VM.

b) Criação da entidade WorkArea:001
	•	Uma requisição HTTP (NGSI v2) cria a entidade inicial com os atributos noiseLevel, status, alert e alertMessage.

c) Atualização de atributos
	•	A cada ciclo, o ESP32 envia um payload JSON com o nível de ruído e o estado do ambiente para o Orion, que atualiza a entidade WorkArea:001.

d) Evidências
	•	As evidências da integração com o FIWARE serão apresentadas em prints de tela, incluindo:
• consulta à entidade WorkArea:001 no Orion (resposta JSON atualizada);
• terminal/console mostrando as leituras de ruído e mensagens;
• tela do simulador Wokwi com o display OLED exibindo os dados do QuietOffice.

Esses prints comprovam que o ESP32 está enviando dados ao Orion e que os valores de noiseLevel, status, alert e alertMessage estão sendo atualizados conforme o ruído simulado.

⸻

	6.	Itens de entrega

	•	Repositório GitHub com código-fonte (sketch do ESP32) e este README.
	•	Link público da simulação Wokwi (ESP32 + OLED + FIWARE).
	•	Vídeo de até 3 minutos, apresentando:
• cenário de uso;
• arquitetura da solução;
• principais desafios e diferenciais;
• demonstração do sistema em funcionamento (Wokwi, display OLED e consulta ao FIWARE).
	•	Prints de tela comprovando a integração com o FIWARE Orion (conforme descrito na seção 5).
