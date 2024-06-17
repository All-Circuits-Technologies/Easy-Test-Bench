<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR">
<context>
    <name>ACallScriptViaSerialLink</name>
    <message>
        <source>An error occurred when calling the linked script. The error code is %1, details of the error: %2.</source>
        <translation type="vanished">Une erreur s&apos;est produite lors de l&apos;appel du script. Le code d&apos;erreur est %1, le détail de l&apos;erreur est: %2.</translation>
    </message>
    <message>
        <source>An error occurred when calling the linked script. The error code is %1.</source>
        <translation type="vanished">Une erreur s&apos;est produite lors de l&apos;appel du script. Le code d&apos;erreur est %1.</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/acallscriptviaseriallink.cpp" line="207"/>
        <source>(TR) An error occurred when calling the linked script. The error code is %1, details of the error: %2.</source>
        <translation>Une erreur s&apos;est produite lors de l&apos;appel du script. Le code d&apos;erreur est %1, le détail de l&apos;erreur est: %2.</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/acallscriptviaseriallink.cpp" line="217"/>
        <source>(TR) An error occurred when calling the linked script. The error code is %1.</source>
        <translation>Une erreur s&apos;est produite lors de l&apos;appel du script. Le code d&apos;erreur est %1.</translation>
    </message>
</context>
<context>
    <name>ASerialLinkProbe</name>
    <message>
        <source>Should we die if not found</source>
        <translation type="vanished">Faut-il interrompre si non trouvé</translation>
    </message>
    <message>
        <source>Serial port name (ex: COM1)</source>
        <translation type="vanished">Nom du port série (ex: COM1)</translation>
    </message>
    <message>
        <source>Serial port SN</source>
        <translation type="vanished">Port série: numéro de série</translation>
    </message>
    <message>
        <source>Serial port USB Vendor ID</source>
        <translation type="vanished">Port série: ID vendeur de l&apos;USB</translation>
    </message>
    <message>
        <source>Serial port USB Product ID</source>
        <translation type="vanished">Port série: ID produit de l&apos;USB</translation>
    </message>
    <message>
        <source>Serial port description</source>
        <translation type="vanished">Description du port série</translation>
    </message>
    <message>
        <source>Serial port manufacturer</source>
        <translation type="vanished">Port série: fabriquant</translation>
    </message>
    <message>
        <source>Serial port system location</source>
        <translation type="vanished">Port série: emplacement sur le système</translation>
    </message>
    <message>
        <source>USB vendor ID is out of uint16 range</source>
        <translation type="vanished">L&apos;id vendeur de l&apos;USB est trop grand pour un uint16</translation>
    </message>
    <message>
        <source>USB product ID is out of uint16 range</source>
        <translation type="vanished">L&apos;id produit de l&apos;USB est trop grand pour un uint16</translation>
    </message>
    <message>
        <source>Is serial link found</source>
        <translation type="vanished">Port série trouvé</translation>
    </message>
    <message>
        <source>Mandatory serial port not found</source>
        <translation type="vanished">Port série obligatoire non trouvé</translation>
    </message>
</context>
<context>
    <name>ASerialLinkSetup</name>
    <message>
        <source>Wanted baudrate (ex: 115200)</source>
        <translation type="vanished">Baudrate souhaité (ex: 115200)</translation>
    </message>
    <message>
        <source>Enable XON/XOFF</source>
        <translation type="vanished">Activer XON/XOFF</translation>
    </message>
    <message>
        <source>Enabled CTS/RTS</source>
        <translation type="vanished">Activer CTS/RTS</translation>
    </message>
    <message>
        <source>Baudrate must be a positive integer</source>
        <translation type="vanished">Le baudrate doit être un entier positif</translation>
    </message>
    <message>
        <source>Software and hardware flow control can not be enabled concurrently</source>
        <translation type="vanished">Les contrôles de flux logiciel et matériel ne peuvent pas être activés en même temps</translation>
    </message>
    <message>
        <source>Serial port name (ex: COM1)</source>
        <translation type="vanished">Nom du port série (ex: COM1)</translation>
    </message>
    <message>
        <source>Missing required serial port name</source>
        <translation type="vanished">Le nom obligatoire du port  série est manquant</translation>
    </message>
    <message>
        <source>Serial port name must not be empty</source>
        <translation type="vanished">Le nom du port série ne peut pas être vide</translation>
    </message>
    <message>
        <source>Serial port %1 not found on the system</source>
        <translation type="vanished">Port série %1, introuvable sur le système</translation>
    </message>
    <message>
        <source>Failed to create serial port %1</source>
        <translation type="vanished">Echec de la création du port série %1</translation>
    </message>
    <message>
        <source>Serial port %1 rejected baudrate %2</source>
        <translation type="vanished">Le port série %1 a rejeté le baudrate %2</translation>
    </message>
    <message>
        <source>Serial port %1 rejected wanted flow contol</source>
        <translation type="vanished">Le port série %1 a rejeté le contrôle de flux</translation>
    </message>
</context>
<context>
    <name>ASerialLinkWriteRead</name>
    <message>
        <source>Given expected pattern is invalid</source>
        <translation type="vanished">Le motif donné est invalide</translation>
    </message>
    <message>
        <source>Failure pattern must be either empty or valid</source>
        <translation type="vanished">Le motif d&apos;échec doit être vide ou valide</translation>
    </message>
    <message>
        <source>Missing required message input</source>
        <translation type="vanished">Il manque le message à envoyer</translation>
    </message>
    <message>
        <source>A problem occurred when tried to send message to serial port: %1</source>
        <translation type="vanished">Un problème s&apos;est produit lors de la tentative d&apos;envoi d&apos;un message sur le port série : %1</translation>
    </message>
    <message>
        <source>Timeout must be a positive integer number or -1 to deactivate the timeout</source>
        <translation type="vanished">Le délai d&apos;attente doit être un entier positif ( -1 pour le désactiver)</translation>
    </message>
    <message>
        <source>Expected pattern can not be empty when expecting a reply</source>
        <translation type="vanished">Le motif du résultat ne peut pas être vide si une réponse est attendue</translation>
    </message>
    <message>
        <source>Timeout can not be zero when expecting a reply</source>
        <translation type="vanished">Le délai d&apos;attente ne peut pas être 0 si une réponse est attendue</translation>
    </message>
    <message>
        <source>Is expected success-line found</source>
        <translation type="vanished">Résultat positif attendu</translation>
    </message>
    <message>
        <source>Matched line if any (success or failure match)</source>
        <translation type="vanished">Correspondance (succès ou échec)</translation>
    </message>
    <message>
        <source>Missing required serial port name input</source>
        <translation type="vanished">Le nom obligatoire du port  série est manquant</translation>
    </message>
    <message>
        <source>Missing required message to send input</source>
        <translation type="vanished">Le message à envoyer est manquant (obligatoire)</translation>
    </message>
    <message>
        <source>Serial port %1 is not yet set up</source>
        <translation type="vanished">Le port série %1 n&apos;est pas encore configuré</translation>
    </message>
    <message>
        <source>Expected success pattern (regexp string) (ex:&quot;&lt;A.*&gt;&quot;)</source>
        <translation type="vanished">Motif du résultat attendu (expression régulière) (ex:&lt;A.*&gt;)</translation>
    </message>
    <message>
        <source>Possible failure pattern (regexp string) (ex:&quot;.*err.*&quot;)</source>
        <translation type="vanished">Motif déchec possible (expression régulière) (ex:&quot;.*err.*&quot;)</translation>
    </message>
    <message>
        <source>Maximum time to wait for a line matching expected or failure pattern, if -1 no timeout</source>
        <translation type="vanished">Temps maximum d&apos;attente pour qu&apos;une ligne corresponde à un motif d&apos;échec ou de réussite, si -1, pas de délai</translation>
    </message>
    <message>
        <source>End of line string (defaults to LF)</source>
        <translation type="vanished">String de fin de ligne ( LF par défaut)</translation>
    </message>
    <message>
        <source>Should received lines be trimmed before verifications</source>
        <translation type="vanished">Faut-il enlever les espaces supplémentaires des lignes avant vérification</translation>
    </message>
    <message>
        <source>Serial port name (ex: COM1)</source>
        <translation type="vanished">Nom du port série (ex: COM1)</translation>
    </message>
    <message>
        <source>Message to send</source>
        <translation type="vanished">Message à envoyer</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="180"/>
        <source>(TR) Given expected pattern is invalid</source>
        <translation>Le motif donné est invalide</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="201"/>
        <source>(TR) Failure pattern must be either empty or valid</source>
        <translation>Le motif d&apos;échec doit être vide ou valide</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="218"/>
        <source>(TR) Timeout can not be zero when expecting a reply</source>
        <translation>Le délai d&apos;attente ne peut pas être 0 si une réponse est attendue</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="490"/>
        <source>(TR) Missing required message input</source>
        <translation>Il manque le message à envoyer</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="530"/>
        <source>(TR) Missing required serial port name input</source>
        <translation>Le nom obligatoire du port  série est manquant</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="540"/>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="560"/>
        <source>(TR) Serial port %1 is not yet set up</source>
        <translation>Le port série %1 n&apos;est pas encore configuré</translation>
    </message>
    <message>
        <location filename="../serial-link-lib/aseriallinkwriteread.cpp" line="593"/>
        <source>(TR) A problem occurred when tried to send message to serial port: %1</source>
        <translation>Un problème s&apos;est produit lors de la tentative d&apos;envoi d&apos;un message sur le port série : %1</translation>
    </message>
</context>
<context>
    <name>AutoAnsSerialLinkProcess</name>
    <message>
        <location filename="autoanswer/processes/autoansseriallinkprocess.cpp" line="276"/>
        <source>(TR) Serial port %1 is not yet set up</source>
        <oldsource>Serial port %1 is not yet set up</oldsource>
        <translation>Le port série %1 n&apos;est pas encore configuré</translation>
    </message>
</context>
<context>
    <name>DefaultSequenceInstanceModule</name>
    <message>
        <source>The sequence module: %1, has ended in error</source>
        <translation type="vanished">Le module: %1, de la séquence s&apos;est terminé en erreur</translation>
    </message>
    <message>
        <location filename="../abstractpluginelements/defaultsequenceinstancemodule.cpp" line="193"/>
        <source>(TR) The sequence module: %1, has ended in error</source>
        <translation>Le module: %1, de la séquence s&apos;est terminé en erreur</translation>
    </message>
</context>
<context>
    <name>DetectFromScriptLogs</name>
    <message>
        <source>Detection has been cancelled by user</source>
        <translation type="vanished">La détection a été annulée par l&apos;utilisateur</translation>
    </message>
    <message>
        <source>Put the board on the test bed</source>
        <oldsource>Wait for user to put the element on the test bed</oldsource>
        <translation type="vanished">Placez la carte sur le banc de test</translation>
    </message>
    <message>
        <source>Take the board out of the test bed.</source>
        <oldsource>Wait for user to take the element out of the test bed.</oldsource>
        <translation type="vanished">Enlevez la carte du banc de test.</translation>
    </message>
</context>
<context>
    <name>ErrorCause</name>
    <message>
        <source>Unknown error cause</source>
        <translation type="vanished">La cause de l&apos;erreur est inconnue</translation>
    </message>
    <message>
        <source>There are two output node ports with the same name: &apos;%1&apos;, but with a different managed type</source>
        <translation type="vanished">Il y a deux &quot;ouptut node ports&quot; avec le même nom : &apos;%1&apos;, mais ayant un type différent</translation>
    </message>
    <message>
        <source>There are two output node ports with the same name: &apos;%1&apos;, but with different units</source>
        <translation type="vanished">Il y a deux &quot;ouptut node ports&quot; avec le même nom : &apos;%1&apos;, mais ayant une unité différente</translation>
    </message>
    <message>
        <source>There are two output node ports with the same name: &apos;%1&apos;, but with a different variable attached, one is: &apos;%2&apos; , and the other is: &apos;%3&apos;</source>
        <translation type="vanished">Il y a deux &quot;ouptut node ports&quot; avec le même nom : &apos;%1&apos;, mais avec une variable attachée différente, un est: &apos;%2&apos; et l&apos;autre est : &apos;%3&apos;</translation>
    </message>
    <message>
        <source>There are two input node ports with the same name: &apos;%1, but with a different reference attached, one is: &apos;%2, and the other is: &apos;%3&apos;</source>
        <translation type="vanished">Il y a deux &quot;input node ports&quot; avec le même nom : &apos;%1&apos;, mais avec une référence attachée différente, un est: &apos;%2&apos; et l&apos;autre est : &apos;%3&apos;</translation>
    </message>
    <message>
        <source>The join node : &apos;%1&apos; does not exist or is not a join node</source>
        <translation type="vanished">Le noeud &apos;join&apos; : &apos;%1&apos; n&apos;existe pas ou n&apos;est pas un noeud &apos;join&apos;</translation>
    </message>
    <message>
        <source>First node is unavailable because one element of the sequence file isn&apos;t well formed</source>
        <translation type="vanished">Le premier noeud n&apos;est pas disponible parce qu&apos;un élément de la séquence n&apos;est pas bien formé</translation>
    </message>
    <message>
        <source>The attribute : &apos;%1&apos; of the first node is missing</source>
        <translation type="vanished">L&apos;attribut : &apos;%1&apos; du premier noeud est manquant</translation>
    </message>
    <message>
        <source>First node has a unknown type</source>
        <translation type="vanished">Le premier noeud est d&apos;un type inconnu</translation>
    </message>
    <message>
        <source>Invalid transition</source>
        <translation type="vanished">Transition invalide</translation>
    </message>
    <message>
        <source>Targeted node unavailable</source>
        <translation type="vanished">Le noeud visé n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>A referenced node can&apos;t be targeted</source>
        <translation type="vanished">Un noeud référencé ne peut pas être visé</translation>
    </message>
    <message>
        <source>Elements unavailable</source>
        <translation type="vanished">Les éléments sont indisponibles</translation>
    </message>
    <message>
        <source>In the node: &apos;%1&apos;, the decisional operator: &apos;%2&apos; is not known</source>
        <translation type="vanished">Dans le noeud : &apos;%1&apos;, l&apos;opérateur de décision : &apos;%2&apos; est inconnu</translation>
    </message>
    <message>
        <source>Transition unavailable</source>
        <translation type="vanished">La transition n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>The node targeted: &apos;%1&apos;, doesn&apos;t exist</source>
        <translation type="vanished">Le noeud visé : &apos;%1&apos;, n&apos;existe pas</translation>
    </message>
    <message>
        <source>Module Parameters error : %1</source>
        <translation type="vanished">Une erreur de paramètre de module est survenu : %1</translation>
    </message>
    <message>
        <source>Module parameters format unavailable</source>
        <translation type="vanished">Le format du paramètre de module n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>Module parameters unavailable</source>
        <translation type="vanished">Les paramètres de module ne sont pas disponibles</translation>
    </message>
    <message>
        <source>Module attribute : &apos;%1&apos; unavailable</source>
        <translation type="vanished">L&apos;attribut du module : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>The bank interface linked to the node : &apos;%1&apos; is unavailable</source>
        <translation type="vanished">L&apos;interface du plugin lié au noeud : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>Can&apos;t get the name of the bank interface linked to the node: &apos;%1&apos;</source>
        <translation type="vanished">Impossible de récupérer le nom de l&apos;interface du plugin lié au noeud : &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Can&apos;t get the name of the module linked to the node : &apos;%1&apos;</source>
        <translation type="vanished">Impossible de récupérer le nom du module lié au noeud : &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Can&apos;t create the sequence module instance named : &apos;%1&apos; , in &apos;%2&apos;</source>
        <translation type="vanished">Impossible de créer l&apos;instance du module de séquence nommé : &apos;%1&apos;, dans &apos;%2&apos;</translation>
    </message>
    <message>
        <source>The bank : &apos;%1&apos; , linked to the sequence module : &apos;%2&apos;, doesn&apos;t exist</source>
        <translation type="vanished">Le plugin : &apos;%1&apos;, lié au module de séquence : &apos;%2&apos;, n&apos;existe pas</translation>
    </message>
    <message>
        <source>The fork node: &apos;%1&apos; doesn&apos;t exist or it&apos;s not a fork node</source>
        <translation type="vanished">Le noeud &apos;fork&apos; : &apos;%1&apos; n&apos;existe pas ou n&apos;est pas un noeud &apos;fork&apos;</translation>
    </message>
    <message>
        <source>The JoinNode: &apos;%1&apos;, is linked to the ForkNode : &apos;%2&apos;, but the ForkNode has already been connected to the JoinNode: &apos;%3&apos;</source>
        <translation type="vanished">Le noeud &apos;Join&apos; : &apos;%1&apos;, est lié au noeud &apos;Fork&apos; : &apos;%2&apos;, mais le noeud &apos;Fork&apos; est déjà connecté au noeud &apos;Join&apos; : &apos;%3&apos;</translation>
    </message>
    <message>
        <source>The JoinNode: &apos;%1&apos;, is already linked to the ForkNode: &apos;%2&apos;</source>
        <translation type="vanished">Le noeud &apos;Join&apos; : &apos;%1&apos;, est déjà lié au noeud &apos;Fork&apos; : &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Some node elements in the file are not objects</source>
        <translation type="vanished">Certains éléments de noeud dans le fichier ne sont pas des objets</translation>
    </message>
    <message>
        <source>The node found is not a sequence node: &apos;%1&apos;</source>
        <translation type="vanished">Le noeud trouvé n&apos;est pas un noeud de séquence : &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Attribute : &apos;%1&apos; unavailable</source>
        <translation type="vanished">L&apos;attribut : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>A node can&apos;t target itself</source>
        <translation type="vanished">Un noeud ne peut pas se viser lui-même</translation>
    </message>
    <message>
        <source>The type of a node is unavailable</source>
        <translation type="vanished">Le type du noeud n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>Error with the output: &apos;%1&apos;, of the group node:&apos;%2&apos;, it&apos;s not a boolean</source>
        <translation type="vanished">L&apos;erreur avec le &apos;output&apos; : &apos;%1&apos;, du neud &apos;group&apos; : &apos;%2&apos;, n&apos;est pas un booléen</translation>
    </message>
    <message>
        <source>The output: %1, is compulsory in the group node: &apos;%2&apos;, because the attribute: &apos;%3&apos;, is equal to false</source>
        <translation type="vanished">Le &apos;output&apos; : &apos;%1&apos;, est obligatoire dans le noeud &apos;group&apos; : &apos;%2&apos;, parce que l&apos;attribut : &apos;%3&apos; vaut &apos;false&apos;</translation>
    </message>
    <message>
        <source>Internal nodes Sequence: &apos;%1&apos; unavailable</source>
        <translation type="vanished">Le noeud de séquence interne : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>Internal nodes Sequence: &apos;%1&apos; not found at entry: &apos;%2&apos;</source>
        <translation type="vanished">Le noeud de séquence interne : &apos;%1&apos; n&apos;est pas trouvé à l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Internal reference node name unavailable</source>
        <translation type="vanished">Le nom du neud de &apos;reference&apos; interne n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>Can&apos;t find the output named: &apos;%1&apos;for the entry: &apos;%2&apos;</source>
        <translation type="vanished">Ne peut pas trouver le &apos;output&apos; nommé : &apos;%1&apos;, pour l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Can&apos;t find the input variable named: &apos;%1&apos;for the entry: &apos;%2&apos;</source>
        <translation type="vanished">Ne peut pas trouver la variable &apos;input&apos; nommé : &apos;%1&apos; pour l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <source>The node port: &apos;%1&apos; (type: &apos;%2&apos;, unit: &apos;%3&apos; ), and the reference node: &apos;%4 (type: &apos;%5&apos;, unit: &apos;%6&apos;), are incompatible</source>
        <translation type="vanished">Le &apos;port&apos; : &apos;%1&apos; (type : &apos;%2&apos;, unité : &apos;%3&apos;), et la &apos;reference&apos; : &apos;%4&apos; (type : &apos;%5&apos;, unité :&apos;%6&apos;) sont incompatibles</translation>
    </message>
    <message>
        <source>Reference node name unavailable</source>
        <translation type="vanished">Le nom du noeud &apos;reference&apos; nest pas disponible</translation>
    </message>
    <message>
        <source>External node: &apos;%1&apos; unavailable</source>
        <translation type="vanished">Le noeud externe : &apos;%1&apos;, n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>Can&apos;t find the node reference named: &apos;%1&apos; for the entry : &apos;%2&apos;</source>
        <translation type="vanished">Ne peut pas trouver le noeud de référence nommé : &apos;%1&apos; pour l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Parent instance name unavailable</source>
        <translation type="vanished">Le nom de l&apos;instance parente n&apos;est pas disponible</translation>
    </message>
    <message>
        <source>Attribute : &apos;%2&apos; has an unknown type &apos;%1&apos;</source>
        <translation type="vanished">L&apos;attribut : &apos;%2&apos; a un type inconnu : &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Unknown type managed: &apos;%1&apos;</source>
        <translation type="vanished">Le type suivant est inconnu : &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Value of type: &apos;%1&apos;, unavailable at attribute: &apos;%2&apos;</source>
        <translation type="vanished">La valeur du type : &apos;%1&apos;, n&apos;est pas disponible pour l&apos;attribut : &apos;%2&apos;</translation>
    </message>
    <message>
        <source>Value unavailable at attribute: &apos;%1&apos;</source>
        <translation type="vanished">La valeur n&apos;est pas disponible pour l&apos;attribut : &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Value : &apos;%1&apos; can&apos;t be set while the sequence is running</source>
        <translation type="vanished">La valeur : &apos;%1&apos;, ne peut pas être assignée pendant que la séquence fonctionne</translation>
    </message>
    <message>
        <source>Unit power of ten overflow the limit</source>
        <translation type="vanished">L&apos;unité de puissance de dix dépasse la limite</translation>
    </message>
    <message>
        <source>Unit power of ten is too big or is not an integer</source>
        <translation type="vanished">L&apos;unité de puissance de dix est trop grosse ou n&apos;est pas un entier</translation>
    </message>
    <message>
        <source>Unknown node type</source>
        <translation type="vanished">Type de noeud inconnu</translation>
    </message>
    <message>
        <source>The sequence file doesn&apos;t contain the compulsory node: &apos;%1&apos;</source>
        <translation type="vanished">Le fichier de séquence ne contient pas le noeud obligatoire : &apos;%1&apos;</translation>
    </message>
    <message>
        <source>Inputs array is not well formed</source>
        <translation type="vanished">La liste des &apos;inputs&apos; n&apos;est pas bien formée</translation>
    </message>
    <message>
        <source>The file suffix isn&apos;t know</source>
        <translation type="vanished">Le suffixe du fichier n&apos;est pas connu</translation>
    </message>
    <message>
        <source>The sequence file can&apos;t be found at : %1</source>
        <translation type="vanished">Le fichier de séquence ne peut pas être trouvé ici : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="106"/>
        <source>(TR) Unknown error cause</source>
        <translation>La cause de l&apos;erreur est inconnue</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="108"/>
        <source>(TR) There are two output node ports with the same name: &apos;%1&apos;, but with a different managed type</source>
        <translation>Il y a deux &quot;ouptut node ports&quot; avec le même nom : &apos;%1&apos;, mais ayant un type différent</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="113"/>
        <source>(TR) There are two output node ports with the same name: &apos;%1&apos;, but with different units</source>
        <translation>Il y a deux &quot;ouptut node ports&quot; avec le même nom : &apos;%1&apos;, mais ayant une unité différente</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="118"/>
        <source>(TR) There are two output node ports with the same name: &apos;%1&apos;, but with a different variable attached, one is: &apos;%2&apos; , and the other is: &apos;%3&apos;</source>
        <translation>Il y a deux &quot;ouptut node ports&quot; avec le même nom : &apos;%1&apos;, mais avec une variable attachée différente, un est: &apos;%2&apos; et l&apos;autre est : &apos;%3&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="125"/>
        <source>(TR) There are two input node ports with the same name: &apos;%1, but with a different reference attached, one is: &apos;%2, and the other is: &apos;%3&apos;</source>
        <translation>Il y a deux &quot;input node ports&quot; avec le même nom : &apos;%1&apos;, mais avec une référence attachée différente, un est: &apos;%2&apos; et l&apos;autre est : &apos;%3&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="131"/>
        <source>(TR) The join node : &apos;%1&apos; does not exist or is not a join node</source>
        <translation>Le noeud &apos;join&apos; : &apos;%1&apos; n&apos;existe pas ou n&apos;est pas un noeud &apos;join&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="135"/>
        <source>(TR) First node is unavailable because one element of the sequence file isn&apos;t well formed</source>
        <translation>Le premier noeud n&apos;est pas disponible parce qu&apos;un élément de la séquence n&apos;est pas bien formé</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="140"/>
        <source>(TR) The attribute : &apos;%1&apos; of the first node is missing</source>
        <translation>L&apos;attribut : &apos;%1&apos; du premier noeud est manquant</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="143"/>
        <source>(TR) First node has a unknown type</source>
        <translation>Le premier noeud est d&apos;un type inconnu</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="145"/>
        <source>(TR) Invalid transition</source>
        <translation>Transition invalide</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="146"/>
        <source>(TR) Targeted node unavailable</source>
        <translation>Le noeud visé n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="148"/>
        <source>(TR) A referenced node can&apos;t be targeted</source>
        <translation>Un noeud référencé ne peut pas être visé</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="150"/>
        <source>(TR) Elements unavailable</source>
        <translation>Les éléments sont indisponibles</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="153"/>
        <source>(TR) In the node: &apos;%1&apos;, the decisional operator: &apos;%2&apos; is not known</source>
        <translation>Dans le noeud : &apos;%1&apos;, l&apos;opérateur de décision : &apos;%2&apos; est inconnu</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="156"/>
        <source>(TR) Transition unavailable</source>
        <translation>La transition n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="158"/>
        <source>(TR) The node targeted: &apos;%1&apos;, doesn&apos;t exist</source>
        <translation>Le noeud visé : &apos;%1&apos;, n&apos;existe pas</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="160"/>
        <source>(TR) Module Parameters error : %1</source>
        <translation>Une erreur de paramètre de module est survenu : %1</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="164"/>
        <source>(TR) Module parameters format unavailable</source>
        <translation>Le format du paramètre de module n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="166"/>
        <source>(TR) Module parameters unavailable</source>
        <translation>Les paramètres de module ne sont pas disponibles</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="170"/>
        <source>(TR) Module attribute : &apos;%1&apos; unavailable</source>
        <translation>L&apos;attribut du module : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="173"/>
        <source>(TR) The bank interface linked to the node : &apos;%1&apos; is unavailable</source>
        <translation>L&apos;interface du plugin lié au noeud : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="176"/>
        <source>(TR) Can&apos;t get the name of the bank interface linked to the node: &apos;%1&apos;</source>
        <translation>Impossible de récupérer le nom de l&apos;interface du plugin lié au noeud : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="180"/>
        <source>(TR) Can&apos;t get the name of the module linked to the node : &apos;%1&apos;</source>
        <translation>Impossible de récupérer le nom du module lié au noeud : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="185"/>
        <source>(TR) Can&apos;t create the sequence module instance named : &apos;%1&apos; , in &apos;%2&apos;</source>
        <translation>Impossible de créer l&apos;instance du module de séquence nommé : &apos;%1&apos;, dans &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="188"/>
        <source>(TR) The bank : &apos;%1&apos; , linked to the sequence module : &apos;%2&apos;, doesn&apos;t exist</source>
        <translation>Le plugin : &apos;%1&apos;, lié au module de séquence : &apos;%2&apos;, n&apos;existe pas</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="191"/>
        <source>(TR) The fork node: &apos;%1&apos; doesn&apos;t exist or it&apos;s not a fork node</source>
        <translation>Le noeud &apos;fork&apos; : &apos;%1&apos; n&apos;existe pas ou n&apos;est pas un noeud &apos;fork&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="195"/>
        <source>(TR) The JoinNode: &apos;%1&apos;, is linked to the ForkNode: &apos;%2&apos;, but the ForkNode has already been connected to the JoinNode: &apos;%3&apos;</source>
        <translation>Le noeud &apos;Join&apos; : &apos;%1&apos;, est lié au noeud &apos;Fork&apos; : &apos;%2&apos;, mais le noeud &apos;Fork&apos; est déjà connecté au noeud &apos;Join&apos; : &apos;%3&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="201"/>
        <source>(TR) The JoinNode: &apos;%1&apos;, is already linked to the ForkNode: &apos;%2&apos;</source>
        <translation>Le noeud &apos;Join&apos; : &apos;%1&apos;, est déjà lié au noeud &apos;Fork&apos; : &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="204"/>
        <source>(TR) Some node elements in the file are not objects</source>
        <translation>Certains éléments de noeud dans le fichier ne sont pas des objets</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="208"/>
        <source>(TR) The node found is not a sequence node: &apos;%1&apos;</source>
        <translation>Le noeud trouvé n&apos;est pas un noeud de séquence : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="210"/>
        <source>(TR) Attribute : &apos;%1&apos; unavailable</source>
        <translation>L&apos;attribut : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="212"/>
        <source>(TR) A node can&apos;t target itself</source>
        <translation>Un noeud ne peut pas se viser lui-même</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="214"/>
        <source>(TR) The type of a node is unavailable</source>
        <translation>Le type du noeud n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="216"/>
        <source>(TR) Error with the output: &apos;%1&apos;, of the group node:&apos;%2&apos;, it&apos;s not a boolean</source>
        <translation>L&apos;erreur avec le &apos;output&apos; : &apos;%1&apos;, du neud &apos;group&apos; : &apos;%2&apos;, n&apos;est pas un booléen</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="221"/>
        <source>(TR) The output: %1, is compulsory in the group node: &apos;%2&apos;, because the attribute: &apos;%3&apos;, is equal to false</source>
        <translation>Le &apos;output&apos; : &apos;%1&apos;, est obligatoire dans le noeud &apos;group&apos; : &apos;%2&apos;, parce que l&apos;attribut : &apos;%3&apos; vaut &apos;false&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="227"/>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="231"/>
        <source>(TR) Internal nodes Sequence: &apos;%1&apos; unavailable</source>
        <translation>Le noeud de séquence interne : &apos;%1&apos; n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="235"/>
        <source>(TR) Internal nodes Sequence: &apos;%1&apos; not found at entry: &apos;%2&apos;</source>
        <translation>Le noeud de séquence interne : &apos;%1&apos; n&apos;est pas trouvé à l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="239"/>
        <source>(TR) Internal reference node name unavailable</source>
        <translation>Le nom du neud de &apos;reference&apos; interne n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="241"/>
        <source>(TR) Can&apos;t find the output named: &apos;%1&apos;for the entry: &apos;%2&apos;</source>
        <translation>Ne peut pas trouver le &apos;output&apos; nommé : &apos;%1&apos;, pour l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="244"/>
        <source>(TR) Can&apos;t find the input variable named: &apos;%1&apos;for the entry: &apos;%2&apos;</source>
        <translation>Ne peut pas trouver la variable &apos;input&apos; nommé : &apos;%1&apos; pour l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="248"/>
        <source>(TR) The node port: &apos;%1&apos; (type: &apos;%2&apos;, unit: &apos;%3&apos; ), and the reference node: &apos;%4 (type: &apos;%5&apos;, unit: &apos;%6&apos;), are incompatible</source>
        <translation>Le &apos;port&apos; : &apos;%1&apos; (type : &apos;%2&apos;, unité : &apos;%3&apos;), et la &apos;reference&apos; : &apos;%4&apos; (type : &apos;%5&apos;, unité :&apos;%6&apos;) sont incompatibles</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="254"/>
        <source>(TR) Reference node name unavailable</source>
        <translation>Le nom du noeud &apos;reference&apos; nest pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="257"/>
        <source>(TR) External node: &apos;%1&apos; unavailable</source>
        <translation>Le noeud externe : &apos;%1&apos;, n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="260"/>
        <source>(TR) Can&apos;t find the node reference named: &apos;%1&apos; for the entry : &apos;%2&apos;</source>
        <translation>Ne peut pas trouver le noeud de référence nommé : &apos;%1&apos; pour l&apos;entrée : &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="264"/>
        <source>(TR) Parent instance name unavailable</source>
        <translation>Le nom de l&apos;instance parente n&apos;est pas disponible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="266"/>
        <source>(TR) Attribute : &apos;%2&apos; has an unknown type &apos;%1&apos;</source>
        <translation>L&apos;attribut : &apos;%2&apos; a un type inconnu : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="269"/>
        <source>(TR) Unknown type managed: &apos;%1&apos;</source>
        <translation>Le type suivant est inconnu : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="271"/>
        <source>(TR) Value of type: &apos;%1&apos;, unavailable at attribute: &apos;%2&apos;</source>
        <translation>La valeur du type : &apos;%1&apos;, n&apos;est pas disponible pour l&apos;attribut : &apos;%2&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="273"/>
        <source>(TR) Value unavailable at attribute: &apos;%1&apos;</source>
        <translation>La valeur n&apos;est pas disponible pour l&apos;attribut : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="276"/>
        <source>(TR) Value : &apos;%1&apos; can&apos;t be set while the sequence is running</source>
        <translation>La valeur : &apos;%1&apos;, ne peut pas être assignée pendant que la séquence fonctionne</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="279"/>
        <source>(TR) Unit power of ten overflow the limit</source>
        <translation>L&apos;unité de puissance de dix dépasse la limite</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="283"/>
        <source>(TR) Unit power of ten is too big or is not an integer</source>
        <translation>L&apos;unité de puissance de dix est trop grosse ou n&apos;est pas un entier</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="286"/>
        <source>(TR) Unknown node type</source>
        <translation>Type de noeud inconnu</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="287"/>
        <source>(TR) The sequence file doesn&apos;t contain the compulsory node: &apos;%1&apos;</source>
        <translation>Le fichier de séquence ne contient pas le noeud obligatoire : &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="291"/>
        <source>(TR) Inputs array is not well formed</source>
        <translation>La liste des &apos;inputs&apos; n&apos;est pas bien formée</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="293"/>
        <source>(TR) The file suffix isn&apos;t known</source>
        <translation>Le suffixe du fichier n&apos;est pas connu</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errorcause.hpp" line="296"/>
        <source>(TR) The sequence file can&apos;t be found at : %1</source>
        <translation>Le fichier de séquence ne peut pas être trouvé ici : &apos;%1&apos;</translation>
    </message>
</context>
<context>
    <name>ErrorType</name>
    <message>
        <source>Unknown error type</source>
        <translation type="vanished">L&apos;erreur est inconnue</translation>
    </message>
    <message>
        <source>Project file is missing at the location : %1, would you like to remove it from recent projects?</source>
        <translation type="vanished">Le fichier de projet n&apos;est pas à l&apos;endroit attendu : &apos;%1&apos;, voulez-vous le supprimer de la liste des projets récents ?</translation>
    </message>
    <message>
        <source>Sequence file is missing at the location: %1, would you like to remove it from recent projects?</source>
        <translation type="vanished">Le fichier de séquence n&apos;est pas à l&apos;endroit attendu : &apos;%1&apos;, voulez-vous le supprimer de la liste des projets récents ?</translation>
    </message>
    <message>
        <source>The file can&apos;t be read, it is not in JSON format or it is inaccessible</source>
        <translation type="vanished">Le fichier ne peut pas être lu, ce n&apos;est pas un JSON ou il est inaccessible</translation>
    </message>
    <message>
        <source>The sequence file contains one or more errors</source>
        <translation type="vanished">Le fichier de séquence contient une ou plusieurs erreurs</translation>
    </message>
    <message>
        <source>The project file contains one or more errors</source>
        <translation type="vanished">Le fichier de projet contient une ou plusieurs erreurs</translation>
    </message>
    <message>
        <source>The file format: %1, is not supported by the library</source>
        <translation type="vanished">Le format de fichier : %1, n&apos;est pas supporté par la librairie</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errortype.hpp" line="34"/>
        <source>(TR) Unknown error type</source>
        <translation>L&apos;erreur est inconnue</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errortype.hpp" line="35"/>
        <source>(TR) Project file is missing at the location: %1, would you like to remove it from recent projects?</source>
        <translation>Le fichier de projet n&apos;est pas à l&apos;endroit attendu : &apos;%1&apos;, voulez-vous le supprimer de la liste des projets récents ?</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errortype.hpp" line="40"/>
        <source>(TR) Sequence file is missing at the location: %1, would you like to remove it from recent projects?</source>
        <translation>Le fichier de séquence n&apos;est pas à l&apos;endroit attendu : &apos;%1&apos;, voulez-vous le supprimer de la liste des projets récents ?</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errortype.hpp" line="45"/>
        <source>(TR) The file can&apos;t be read, it is not in JSON format or it is inaccessible</source>
        <translation>Le fichier ne peut pas être lu, ce n&apos;est pas un JSON ou il est inaccessible</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errortype.hpp" line="49"/>
        <source>(TR) The sequence file contains one or more errors</source>
        <translation>Le fichier de séquence contient une ou plusieurs erreurs</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errortype.hpp" line="52"/>
        <source>(TR) The project file contains one or more errors</source>
        <translation>Le fichier de projet contient une ou plusieurs erreurs</translation>
    </message>
    <message>
        <location filename="../../testbedcore/errors/errortype.hpp" line="55"/>
        <source>(TR) The file format: %1, is not supported by the library</source>
        <translation>Le format de fichier : %1, n&apos;est pas supporté par la librairie</translation>
    </message>
</context>
<context>
    <name>HmiDisplaySimpleTestView</name>
    <message>
        <source>Ok</source>
        <translation type="vanished">Conforme</translation>
    </message>
    <message>
        <source>Not ok</source>
        <translation type="vanished">Non conforme</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="vanished">Annuler</translation>
    </message>
    <message>
        <location filename="../hmi-helper-lib/hmidisplaysimpletestview.hpp" line="113"/>
        <source>(TR) Ok</source>
        <translation>Conforme</translation>
    </message>
    <message>
        <location filename="../hmi-helper-lib/hmidisplaysimpletestview.hpp" line="119"/>
        <source>(TR) Not ok</source>
        <translation>Non conforme</translation>
    </message>
    <message>
        <location filename="../hmi-helper-lib/hmidisplaysimpletestview.hpp" line="125"/>
        <source>(TR) Cancel</source>
        <translation>Annuler</translation>
    </message>
</context>
<context>
    <name>LogMsgType</name>
    <message>
        <source>LogDebugMsg</source>
        <translation type="vanished">Débug</translation>
    </message>
    <message>
        <source>LogInfoMsg</source>
        <translation type="vanished">Info</translation>
    </message>
    <message>
        <source>LogWarningMsg</source>
        <translation type="vanished">Alerte</translation>
    </message>
    <message>
        <source>LogCriticalMsg</source>
        <translation type="vanished">Critique</translation>
    </message>
    <message>
        <source>LogFatalMsg</source>
        <translation type="vanished">Fatal</translation>
    </message>
    <message>
        <source>D</source>
        <translation type="vanished">D</translation>
    </message>
    <message>
        <source>I</source>
        <translation type="vanished">I</translation>
    </message>
    <message>
        <source>W</source>
        <translation type="vanished">A</translation>
    </message>
    <message>
        <source>C</source>
        <translation type="vanished">C</translation>
    </message>
    <message>
        <source>F</source>
        <translation type="vanished">F</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="98"/>
        <source>(TR) LogDebugMsg</source>
        <translation>Débug</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="99"/>
        <source>(TR) LogInfoMsg</source>
        <translation>Info</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="100"/>
        <source>(TR) LogWarningMsg</source>
        <translation>Alerte</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="101"/>
        <source>(TR) LogCriticalMsg</source>
        <translation>Critique</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="102"/>
        <source>(TR) LogFatalMsg</source>
        <translation>Fatal</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="104"/>
        <source>(TR) D</source>
        <translation>D</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="105"/>
        <source>(TR) I</source>
        <translation>I</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="106"/>
        <source>(TR) W</source>
        <translation>A</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="107"/>
        <source>(TR) C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/logsutility/logmsgtype.hpp" line="108"/>
        <source>(TR) F</source>
        <translation>F</translation>
    </message>
</context>
<context>
    <name>MixinBoardDetection</name>
    <message>
        <source>Put the board on the test bed.</source>
        <translation type="vanished">Placez la carte dans le banc de test.</translation>
    </message>
    <message>
        <source>Put the board on the test bed.

If you have put the board on the bench and this view is still displayed, click on button: &apos;%1&apos;.</source>
        <translation type="vanished">Placez la carte dans le banc de test.

Si vous avez déjà placé la carte dans le banc et que cette vue est toujours affichée, cliquez sur le bouton : &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Take the board out of the test bed.</source>
        <translation type="vanished">Enlevez la carte du banc de test.</translation>
    </message>
    <message>
        <source>Error: on bench</source>
        <translation type="vanished">Erreur : dans le banc</translation>
    </message>
    <message>
        <source>Detection has been cancelled by user</source>
        <translation type="vanished">La détection a été annulée par l&apos;utilisateur</translation>
    </message>
    <message>
        <location filename="../detection-helper-lib/modules/mixinboarddetection.hpp" line="150"/>
        <source>(TR) Put the board on the test bed.</source>
        <translation>Placez la carte dans le banc de test.</translation>
    </message>
    <message>
        <location filename="../detection-helper-lib/modules/mixinboarddetection.hpp" line="152"/>
        <source>(TR) Put the board on the test bed.

If you have put the board on the bench and this view is still displayed, click on button: &apos;%1&apos;.</source>
        <translation>Placez la carte dans le banc de test.

Si vous avez déjà placé la carte dans le banc et que cette vue est toujours affichée, cliquez sur le bouton : &apos;%1&apos;.</translation>
    </message>
    <message>
        <location filename="../detection-helper-lib/modules/mixinboarddetection.hpp" line="156"/>
        <source>(TR) Take the board out of the test bed.</source>
        <translation>Enlevez la carte du banc de test.</translation>
    </message>
    <message>
        <location filename="../detection-helper-lib/modules/mixinboarddetection.hpp" line="159"/>
        <source>(TR) Error: on bench</source>
        <translation>Erreur : dans le banc</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <source>The attribute: %1, of the object: %2, hasn&apos;t been found</source>
        <translation type="vanished">L&apos;attribut: %1, de l&apos;objet: %2, n&apos;a pas été trouvé</translation>
    </message>
    <message>
        <source>Can&apos;t convert the JsonArray element: %1, to JsonObject</source>
        <translation type="vanished">Impossible de convertir l&apos;élément: %1, du JsonArray en JsonObject</translation>
    </message>
    <message>
        <source>The wanted object: %1, hasn&apos;t been found</source>
        <translation type="vanished">L&apos;objet attendu: %1, n&apos;a pas été trouvé</translation>
    </message>
    <message>
        <source>The attribute: %1, of the object: %2, doesn&apos;t exist</source>
        <translation type="vanished">L&apos;attribut : %1, de l&apos;objet : %2, n&apos;existe pas</translation>
    </message>
    <message>
        <source>The attribute: %1, of the object: %2, doesn&apos;texist</source>
        <translation type="vanished">L&apos;attribut: %1 de l&apos;objet: %2 n&apos;existe pas</translation>
    </message>
    <message>
        <source>The attribute: %1, of the object: %2, isn&apos;t at the rigth type</source>
        <translation type="vanished">L&apos;attribut: %1, de l&apos;objet: %2 n&apos;est pas du bon type</translation>
    </message>
    <message>
        <source>The attribute: %1, of the object: %2, isn&apos;t a JsonObject </source>
        <translation type="vanished">L&apos;attribut: %1, de l&apos;objet: %2, n&apos;est pas un JsonObject </translation>
    </message>
    <message>
        <source>The attribute: %1, of the object: %2, isn&apos;t a JsonArray </source>
        <translation type="vanished">L&apos;attribut: %1 de l&apos;objet: %2 n&apos;est pas un JsonArray </translation>
    </message>
    <message>
        <source>Can&apos;t parse the attribute: %1, of the object: %2</source>
        <translation type="vanished">Impossible de parser l&apos;attribut: %1 de l&apos;objet: %2</translation>
    </message>
    <message>
        <location filename="../../actlibs/qtutilities/numberutility/number.cpp" line="182"/>
        <source>,</source>
        <extracomment>Character used as decimal separator</extracomment>
        <translation>,</translation>
    </message>
    <message>
        <source>Detection has been cancelled by user</source>
        <translation type="vanished">La détection a été annulée par l&apos;utilisateur</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="547"/>
        <source>(TR) The attribute: %1, of the object: %2, hasn&apos;t been found</source>
        <translation>L&apos;attribut: %1, de l&apos;objet: %2, n&apos;a pas été trouvé</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="559"/>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="751"/>
        <source>(TR) Can&apos;t convert the JsonArray element: %1, to JsonObject</source>
        <translation>Impossible de convertir l&apos;élément: %1, du JsonArray en JsonObject</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="592"/>
        <source>(TR) The wanted object: %1, hasn&apos;t been found</source>
        <translation>L&apos;objet attendu: %1, n&apos;a pas été trouvé</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="622"/>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="655"/>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="693"/>
        <source>(TR) The attribute: %1, of the object: %2, doesn&apos;t exist</source>
        <translation>L&apos;attribut : %1, de l&apos;objet : %2, n&apos;existe pas</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="630"/>
        <source>(TR) The attribute: %1, of the object: %2, isn&apos;t at the rigth type</source>
        <translation>L&apos;attribut: %1, de l&apos;objet: %2 n&apos;est pas du bon type</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="665"/>
        <source>(TR) The attribute: %1, of the object: %2, isn&apos;t a JsonObject</source>
        <translation>L&apos;attribut: %1, de l&apos;objet: %2, n&apos;est pas un JsonObject</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="703"/>
        <source>(TR) The attribute: %1, of the object: %2, isn&apos;t a JsonArray</source>
        <translation>L&apos;attribut: %1 de l&apos;objet: %2 n&apos;est pas un JsonArray</translation>
    </message>
    <message>
        <location filename="../../testbedcore/parsers/bankjsonparser.hpp" line="774"/>
        <source>(TR) Can&apos;t parse the attribute: %1, of the object: %2</source>
        <translation>Impossible de parser l&apos;attribut: %1 de l&apos;objet: %2</translation>
    </message>
</context>
<context>
    <name>SerialLinkProbe</name>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="77"/>
        <source>(TR) Mandatory serial port not found</source>
        <oldsource>Mandatory serial port not found</oldsource>
        <translation>Port série obligatoire non trouvé</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="91"/>
        <source>(TR) Should we die if not found</source>
        <oldsource>Should we die if not found</oldsource>
        <translation>Faut-il interrompre si non trouvé</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="104"/>
        <location filename="sequence/seriallinkprobe.cpp" line="243"/>
        <source>(TR) Serial port name (ex: COM1)</source>
        <oldsource>Serial port name (ex: COM1)</oldsource>
        <translation>Nom du port série (ex: COM1)</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="117"/>
        <location filename="sequence/seriallinkprobe.cpp" line="251"/>
        <source>(TR) Serial port SN</source>
        <oldsource>Serial port SN</oldsource>
        <translation>Port série: numéro de série</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="130"/>
        <location filename="sequence/seriallinkprobe.cpp" line="258"/>
        <source>(TR) Serial port USB Vendor ID</source>
        <oldsource>Serial port USB Vendor ID</oldsource>
        <translation>Port série: ID vendeur de l&apos;USB</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="151"/>
        <location filename="sequence/seriallinkprobe.cpp" line="265"/>
        <source>(TR) Serial port USB Product ID</source>
        <oldsource>Serial port USB Product ID</oldsource>
        <translation>Port série: ID produit de l&apos;USB</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="171"/>
        <location filename="sequence/seriallinkprobe.cpp" line="273"/>
        <source>(TR) Serial port description</source>
        <oldsource>Serial port description</oldsource>
        <translation>Description du port série</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="184"/>
        <location filename="sequence/seriallinkprobe.cpp" line="281"/>
        <source>(TR) Serial port manufacturer</source>
        <oldsource>Serial port manufacturer</oldsource>
        <translation>Port série: fabriquant</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="197"/>
        <location filename="sequence/seriallinkprobe.cpp" line="288"/>
        <source>(TR) Serial port system location</source>
        <oldsource>Serial port system location</oldsource>
        <translation>Port série: emplacement sur le système</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="209"/>
        <source>This is the nth element to take in the list of found elements</source>
        <translation>Ceci est le enième élément à récupérer dans la liste des éléments trouvés</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="236"/>
        <source>(TR) Is serial link found</source>
        <oldsource>Is serial link found</oldsource>
        <translation>Port série trouvé</translation>
    </message>
    <message>
        <location filename="sequence/seriallinkprobe.cpp" line="295"/>
        <source>(TR) Serial port nth element</source>
        <oldsource>Serial port nth element</oldsource>
        <translation>Enième élément de la liste des ports séries</translation>
    </message>
</context>
<context>
    <name>SerialLinkSetup</name>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="38"/>
        <source>(TR) Wanted baudrate (ex: 115200)</source>
        <oldsource>Wanted baudrate (ex: 115200)</oldsource>
        <translation>Baudrate souhaité (ex: 115200)</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="60"/>
        <source>(TR) Enable XON/XOFF</source>
        <oldsource>Enable XON/XOFF</oldsource>
        <translation>Activer XON/XOFF</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="74"/>
        <source>(TR) Enabled CTS/RTS</source>
        <oldsource>Enabled CTS/RTS</oldsource>
        <translation>Activer CTS/RTS</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="93"/>
        <source>(TR) Serial port name (ex: COM1)</source>
        <oldsource>Serial port name (ex: COM1)</oldsource>
        <translation>Nom du port série (ex: COM1)</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="143"/>
        <source>(TR) Software and hardware flow control can not be enabled concurrently</source>
        <oldsource>Software and hardware flow control can not be enabled concurrently</oldsource>
        <translation>Les contrôles de flux logiciel et matériel ne peuvent pas être activés en même temps</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="174"/>
        <source>(TR) Missing required serial port name</source>
        <translation>Le nom obligatoire du port  série est manquant</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="182"/>
        <source>(TR) Serial port name must not be empty</source>
        <translation>Le nom du port série ne peut pas être vide</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="190"/>
        <source>(TR) Serial port %1 not found on the system</source>
        <translation>Port série %1, introuvable sur le système</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="212"/>
        <source>(TR) A problem occurred when trying to create or get the serial port: %1</source>
        <translation>Un problème s&apos;est produit pendant l&apos;essai de création ou récupération du port série : %1</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="222"/>
        <source>(TR) Serial port %1 rejected baudrate %2</source>
        <translation>Le port série %1 a rejeté le baudrate %2</translation>
    </message>
    <message>
        <location filename="sequence/seriallinksetup.cpp" line="231"/>
        <source>(TR) Serial port %1 rejected wanted flow contol</source>
        <translation>Le port série %1 a rejeté le contrôle de flux</translation>
    </message>
    <message>
        <source>Missing required serial port name</source>
        <translation type="vanished">Le nom obligatoire du port  série est manquant</translation>
    </message>
    <message>
        <source>Serial port name must not be empty</source>
        <translation type="vanished">Le nom du port série ne peut pas être vide</translation>
    </message>
    <message>
        <source>Serial port %1 not found on the system</source>
        <translation type="vanished">Port série %1, introuvable sur le système</translation>
    </message>
    <message>
        <source>A problem occurred when trying to create or get the serial port: %1</source>
        <translation type="vanished">Un problème s&apos;est produit pendant l&apos;essai de création ou récupération du port série : %1</translation>
    </message>
    <message>
        <source>Serial port %1 rejected baudrate %2</source>
        <translation type="vanished">Le port série %1 a rejeté le baudrate %2</translation>
    </message>
    <message>
        <source>Serial port %1 rejected wanted flow contol</source>
        <translation type="vanished">Le port série %1 a rejeté le contrôle de flux</translation>
    </message>
</context>
<context>
    <name>StartSerialAutoAnswer</name>
    <message>
        <location filename="autoanswer/modules/startserialautoanswer.cpp" line="256"/>
        <source>(TR) A problem occurred while trying to start the auto answer process</source>
        <oldsource>A problem occurred while trying to start the auto answer process</oldsource>
        <translation>Un problème s&apos;est produit pendant le démarrage du processus de réponses auto</translation>
    </message>
</context>
<context>
    <name>StopSerialAutoAnswer</name>
    <message>
        <location filename="autoanswer/modules/stopserialautoanswer.cpp" line="52"/>
        <source>(TR) A problem occurred while trying to stop the auto answer process</source>
        <oldsource>A problem occurred while trying to stop the auto answer process</oldsource>
        <translation>Un problème s&apos;est produit pendant l&apos;arrêt du processus de réponses auto</translation>
    </message>
</context>
<context>
    <name>T</name>
    <message>
        <source>Put the board on the test bed.</source>
        <translation type="vanished">Placez la carte dans le banc de test.</translation>
    </message>
    <message>
        <source>Put the board on the test bed.

If you have put the board on the bench and this view is still displayed, click on button: &apos;%1&apos;.</source>
        <translation type="vanished">Placez la carte dans le banc de test.

Si vous avez déjà placé la carte dans le banc et que cette vue est toujours affichée, cliquez sur le bouton : &apos;%1&apos;.</translation>
    </message>
    <message>
        <source>Take the board out of the test bed.</source>
        <translation type="vanished">Enlevez la carte du banc de test.</translation>
    </message>
    <message>
        <source>Error: on bench</source>
        <translation type="vanished">Erreur : dans le banc</translation>
    </message>
</context>
</TS>
