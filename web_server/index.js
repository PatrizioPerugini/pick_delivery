var ws = new WebSocket('ws://'+self.location.host+'/','pick-protocol');

ws.onmessage = function(event) {
    document.getElementById('msgBox').innerHTML = event.data;
    document.getElementById('outMsg').value='';
}


function sendPacket(packet)
{
    ws.send(packet);
}
