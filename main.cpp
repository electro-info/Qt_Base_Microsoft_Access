#include <QCoreApplication>
#include <QtSql>
#include <QDebug>
#include <Windows.h>    // Pour GetConsoleOutputCP et SetConsoleOutputCP (affichage des accents dans la console)

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Pour affichage correct des accents dans la console
    uint temp = GetConsoleOutputCP();   // Sauvegarde du réglage par défaut
    SetConsoleOutputCP(CP_UTF8);        // Modification du réglage
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    // Instanciation d'un objet QSqlDatabase
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    // Définition des paramètres de connexion à la base de données
    db.setDatabaseName("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=C:\\Users\\sn\\Documents\\Sommets.mdb");

    if(db.open()) {
        qDebug() << "Ok - ouverture de la base de données";

        // Exécution d'une requête
        QSqlQuery requete;
        QString sSql = "SELECT * FROM Sommets";
        if(requete.exec(sSql)) {
            qDebug() << "Ok - requête : " << sSql << "\n";

            // Boucle qui permet de parcourir les enregistrements renvoyés par la requête
            while(requete.next()) {
                // On accède ici aux différents champs par leurs noms, il est également possible
                // d'y accéder par leur index : requete.value(0)
                qDebug() << requete.value("idSommet").toInt() << "\t" << requete.value("Massif").toString()
                         << "\t" << requete.value("Nom").toString() << "\t" << requete.value("Altitude").toInt();
            }
        }
        else {
            qDebug() << "ÉCHEC de la requête";
            // La méthode lastError permet d'afficher un message
            // plus explicite sur les causes de l'erreur
            qDebug() << requete.lastError();
        }
        db.close(); // Fermeture de la base de données
    }
    else {
        qDebug() << "ÉCHEC d'ouverture de la base : " << db.lastError();
    }

    SetConsoleOutputCP(temp);   // Restauration du réglage par défaut

    return a.exec();
}
