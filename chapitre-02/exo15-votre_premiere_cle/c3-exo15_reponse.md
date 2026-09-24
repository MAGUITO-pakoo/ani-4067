# Exercice 15 : votre première clé

## Commande employée

jenga keygen --interactive --alias masalle --output masalle.jks

keytool -genkeypair -alias masalle -keyalg RSA -keysize 2048 -validity 10000 -keystore masalle.jks -storepass ******** -keypass ******** -dname "CN=Jenga User"

## Emplacement de la clé

`C:\Users\pakoo\Desktop\AN-ING4\Keys\masalle.jks` c'est un dossier voisin du dépôt `ani-4067`.

il s'agit du mot de passe de mon pc