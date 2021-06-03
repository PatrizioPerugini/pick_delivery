//var ws = new WebSocket('ws://'+self.location.host+'/','pick-protocol');

var ws = new WebSocket('ws://127.0.0.1:9002/','pick-protocol');
//utilizata per leggere un messaggio ricevuto dal server
ws.onmessage = function(event) {
    //console.debug("WebSocket message received:", event.data);
    alert("tornato mex");
  };


function vai(){
    var tosend=document.getElementById("venire").value+','+document.getElementById("andare").value;
    //ws.send(document.getElementById("venire").value);
    //ws.send(document.getElementById("andare").value);
    ws.send(tosend);
    document.getElementById("venire").value='';
    document.getElementById("andare").value='';
    return 1;
}