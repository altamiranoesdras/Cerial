# cerial
Comunicación serial entre computadoras en lenguaje C

# Ejemplo de uso

--Enviar y recibir texto

./Emisor -p /dev/ttyUSB0 -s hola

./Receptor -p /dev/ttyUSB0 -r

--Enviar y recibir archivos

./Emisor -p /dev/ttyUSB0 -f archivo.txt

./Receptor -p /dev/ttyUSB0 -R nuevo.txt
