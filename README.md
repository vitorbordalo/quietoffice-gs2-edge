QuietOffice – Monitor de Ruído (ESP32 + FIWARE + OLED)
Global Solution 2 – Futuro do Trabalho (FIAP)
Alunos: Vitor Bordalo Correa Guimaraes (RM: 561592) e Lucas Flekner Branquinho (RM: 562262)
Turma: 1ESPH

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

Link vide no Youtube:

⸻

	5.	Integração com FIWARE e evidências 

a) Verificação do Orion
	•	É feita via requisição HTTP para o endpoint /version do Orion na VM.

b) Criação da entidade WorkArea:001
	•	Uma requisição HTTP (NGSI v2) cria a entidade inicial com os atributos noiseLevel, status, alert e alertMessage.

c) Atualização de atributos
	•	A cada ciclo, o ESP32 envia um payload JSON com o nível de ruído e o estado do ambiente para o Orion, que atualiza a entidade WorkArea:001.

d) Evidências
	<img width="1913" height="1003" alt="image" src="https://github.com/user-attachments/assets/5b4bc4a9-2262-4a9d-ae7c-ae7eac35ffe8" />
	<img width="1920" height="584" alt="image" src="https://github.com/user-attachments/assets/b7d80158-7b5e-403a-bd27-a2848e16fc54" />
	<img width="1920" height="1031" alt="image" src="https://github.com/user-attachments/assets/aef16eca-423f-4fbb-a7fb-353d6011c6fd" />




