# 🤖 Edge Computing - Sprint 3 - Tech Mahindra
👋 Olá, bem-vindo ao nosso trabalho da Sprint 3 de Edge Computing! Nós somos a empresa Data Sphere da turma 1ESPH, e é um imenso prazer apresentar este projeto.

![Data Sphere1000x1000](https://github.com/ianmonteirom/CP2-Edge/assets/152393807/0fe80a9b-6290-417d-8367-2abe3824d0b0)
Logo da nossa equipe
## 🔮 O que é a Data Sphere?
A Data Sphere Solutions é uma empresa fictícia que representa nossa equipe, composta pelos seguintes membros:
-  <a href="https://www.linkedin.com/in/artur-alves-tenca-b1ba862b6/">Artur Alves</a> - RM 555171 
- <a href="https://www.linkedin.com/in/giuliana-lucas-85b4532b6/">Giuliana Lucas</a> - RM 557597
- <a href="https://www.linkedin.com/in/ian-monteiro-moreira-a4543a2b7/">Ian Monteiro</a> - RM 558652 
- <a href="https://www.linkedin.com/in/igor-brunelli-ralo-39143a2b7/">Igor Brunelli</a> - RM 555035
- <a href="https://www.linkedin.com/in/matheus-estev%C3%A3o-5248b9238/">Matheus Alcântara</a> - RM 558193

## 👥 Quem é o nosso cliente?
Nosso cliente é a Tech Mahindra, uma empresa indiana multinacional de serviços de tecnologia da informação (TI) e consultoria, com sede em Pune, Índia. Fundada em 1986, faz parte do Grupo Mahindra, um dos principais conglomerados empresariais da Índia.

## 🤔 O Problema que nos foi passado
A Tech Mahindra nos desafiou a aumentar a visibilidade da Fórmula E, uma modalidade de corrida de carros elétricos que não é muito conhecida.

## ✅ A nossa Solução
- Após realizarmos algumas pesquisas de campo com as pessoas, descobrimos que as pessoas mais velhas até conhecem e gostam, mas não possuem tempo para assistir. Já os mais jovens simplesmente não connhecem a Fórmula E.
- Com esses dados, após um brainstorming de ideias, chegamos na ideia de firmar parcerias com escolas, para fins de realizar apresentações para os alunos sobre a Fórmula E, com diversos temas, entre eles:
  - A História da Fórmula E
  - O que são e como funcionam os Carros Elétricos
  - Grandes Pilotos que competiram na modalidade
  - A diferença da Fórmula E para a Fórmula 1
  - E mais
- Além das apresentações, planejamos organizar competições de carrinhos elétricos entre os alunos, estimulando o âmbito competitivo, introduzindo os alunos ao mundo das corridas e saciando a vontade de dirigir carros reais. A primeira competição será livre, com todos os alunos podendo participar. Já a partir da segunda, planejamos exigir uma nota mínima para a participação.
- Planejamos também agendar visitas de nomes importantes da Fórmula E às escolas, como pilotos.
- E por último, planejamos agendar passeios escolares para pistas de corrida, e para marcas que competem na Fórmula E.
- Tudo isso irá introduzir os alunos ao meio da Fórmula E, trazendo uma maior visibilidade a modalidade.

## 💡 Nossa entrega para a Sprint 3 da matéria Edge Computing

### Wokwi
Para esta entrega, desenvolvemos um protótipo de motor de carrinho elétrico no simulador Wokwi, onde é possível controlar sua direção (b: para trás, f: para frente, s: parar) e sua velocidade através do serial do simulador, via comandos:

![image](https://github.com/user-attachments/assets/a24c2465-6566-4395-8e59-9e95b05fd763)

Neste exemplo, foi digitado "f 80", atualizando a direção do motor para frente e sua velocidade para 80.
### Link do Projeto:
- https://wokwi.com/projects/409742956091395073

### Máquina Virtual
Foi hospedada uma máquina virtual com sistema operacional Ubuntu Server na Microsoft Azure, aberta todas as portas necessárias para comunicação e instalado o Fiware Descomplicado, do professor Fábio Cabrini. Isso possibilitará a comunicação do Wokwi com o Postman:

![image](https://github.com/user-attachments/assets/649b2361-9b24-44e7-9d97-52494bcf4f30)

Configuração de portas da máquina virtual:

![image](https://github.com/user-attachments/assets/5a816894-5014-4ac4-a535-e0055ceff7ab)

Github do Fiware Descomplicado:
- https://github.com/fabiocabrini/fiware

### Postman

Além disso, é feito uma conexão com o WiFi do simulador, possibilitando assim o envio e recebimento dos dados de direção e velocidade por uma API. Essa API pode ser gerenciada pelo programa Postman:

![image](https://github.com/user-attachments/assets/de7457e0-7811-44d6-8af4-de45a14768b2)

O Postman facilita a manipulação do JSON, separando cada funcionalidade por abas:

![image](https://github.com/user-attachments/assets/e39a0b50-5900-4e0b-bbe0-1fef2f05c209)

É importante realizar os Health Checks do IOT Agent MQTT, do STH-Comet e do Orion Context Broker:
![image](https://github.com/user-attachments/assets/64d82cd1-ded9-4a06-91c5-7e4053d5362e)
![image](https://github.com/user-attachments/assets/44ee9680-1666-4df7-97db-0fc8211ded5c)
![image](https://github.com/user-attachments/assets/108d2af8-c06c-43a5-a0d4-e792bd6e58b6)
Os Health Checks nos informam se os serviços estão conseguindo comunicar corretamente com o servidor.

![image](https://github.com/user-attachments/assets/e76c43c2-16aa-4647-87cb-63aec2a5d711)

Importante: Definir a variável {{url}} como o IP do servidor.

- Nesta lista de operações, é possível receber os dados de velocidade e direção, bem como enviá-los ao Wokwi.

## Dashboard
Com os dados que o servidor recebe do Wokwi, através de um código em Python ele plota uma Dashboard online, onde qualquer pessoa pode acessar, com dois gráficos: O da velocidade e velocidade média, e o da direção, ambos os gráficos são atualizados em tempo real:
![image](https://github.com/user-attachments/assets/43868954-4093-43cb-b948-802f95a29a1d)



