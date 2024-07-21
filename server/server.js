const express = require('express');
const { SerialPort, ReadlineParser } = require('serialport');
const path = require('path');

const app = express();
const port = 8080;

// Configuración del puerto serial
const serialPort = new SerialPort({
  path: '/dev/ttyUSB0',
  baudRate: 9600
});

const parser = serialPort.pipe(new ReadlineParser({ delimiter: '\r\n' }));

// Arreglo para almacenar los últimos 10 mensajes
const messageQueue = [];

// Leer datos del puerto serial
parser.on('data', (data) => {
  console.log(`Data received: ${data}`);
  messageQueue.push(data);

  // Mantener sólo los últimos 10 mensajes
  if (messageQueue.length > 10) {
    messageQueue.shift();
  }
});

// Servir archivos estáticos de la carpeta public
app.use(express.static('public'));

// Ruta para obtener los datos
app.get('/data', (req, res) => {
  res.json(messageQueue);
});

// Ruta para enviar datos por UART
app.post('/send', express.json(), (req, res) => {
  const { byte } = req.body;
  if (byte !== undefined) {
    serialPort.write(Buffer.from([byte]), (err) => {
      if (err) {
        return res.status(500).send('Error writing to serial port');
      }
      res.send('Byte sent');
    });
  } else {
    res.status(400).send('No byte provided');
  }
});

app.listen(port, () => {
  console.log(`Servidor web corriendo en http://localhost:${port}`);
});
