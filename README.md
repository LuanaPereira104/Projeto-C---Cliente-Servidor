# Projeto-C---Cliente-Servidor

Sistema cliente-servidor desenvolvido em C para a unidade curricular de Sistemas Operativos (2025/2026), implementado em ambiente Linux e baseado em comunicação interprocessos através de named pipes (FIFOs). O cliente envia uma lista de comandos não interativos ao servidor, que os executa de forma concorrente utilizando chamadas ao sistema como fork(), execvp(), open(), read() e write(). O servidor regista ainda o código de saída de cada processo num ficheiro de log, garantindo um histórico completo das execuções.

O projeto segue uma arquitetura simples e robusta, com separação clara entre cliente e servidor, tratamento de erros e utilização exclusiva de mecanismos de baixo nível estudados na UC. A solução foi desenvolvida com foco na compreensão prática dos conceitos de gestão de processos, comunicação entre processos e sincronização, consolidando os conteúdos teóricos abordados nas aulas.
