# pick-delivery
File rilevanti : 
1)main e reader nella cartella server wev
2)pub.cpp
3)stack_navigation.cpp

Nella cartella server web viene tirata su la connessione.
I dati necessari vengono letti da un databaserealizzato in mysql , quindi inviati tramite fifo al pub.cpp.
Una volta ricevute le coordinate viene pubblicato un messaggio sul topic new_goal.
La pubblicazione di questo messaggio viene intercettata, in stack_navigation sono inviate le coordinate al robot.
Tutta la parte di connessione e lettura dal database è effettuata nel reader.
