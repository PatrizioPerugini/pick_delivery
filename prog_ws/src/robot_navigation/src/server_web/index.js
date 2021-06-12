//var ws = new WebSocket('ws://'+self.location.host+'/','pick-protocol');

var ws = new WebSocket('ws://127.0.0.1:9002/','pick-protocol');
//utilizata per leggere un messaggio ricevuto dal server
ws.onmessage = function(event) {
    //console.debug("WebSocket message received:", event.data);
    //alert("tornato mex");
    var log=event.data[0];
    alert("ricevuto " + log);
    if(log=="0"){
        alert("spiacente utente non registrato");
        document.getElementById("nome").value='';
        document.getElementById("password").value='';
    }
    else{
        document.getElementById("login").style.display="none";
        document.getElementById("pick").style.display="block";
    }
  };


function vai(){
    var tosend=document.getElementById("venire").value+','+document.getElementById("andare").value;
    
    ws.send(tosend);
    document.getElementById("venire").value='';
    document.getElementById("andare").value='';
    return 1;
}

function login(){
  var tosend=document.getElementById("nome").value+','+document.getElementById("password").value;
  ws.send(tosend);
}