*This project has been created as part of the 42 curriculum by cpietrza.*

# Codexion

## Description

Codexion est une simulation de programmation concurrente écrite en C. Plusieurs
codeurs partagent un nombre limité de dongles USB. Un codeur a besoin des deux
dongles qui lui sont adjacents pour compiler, puis alterne entre le débogage et
le refactoring.

L'objectif est de coordonner tous les threads sans interblocage, sans mélange
des messages dans les logs et sans accès injuste aux ressources. Un moniteur
arrête la simulation lorsqu'un codeur fait un burnout ou lorsque tous les
codeurs ont effectué le nombre de compilations demandé.

## Instructions

Compilez le programme avec :

```sh
make
```

Lancez-le avec les huit arguments obligatoires :

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown scheduler
```

Toutes les durées sont exprimées en millisecondes. L'ordonnanceur doit être
`fifo` ou `edf`.

Exemples :

```sh
./codexion 5 800 200 100 100 5 20 fifo
./codexion 5 800 200 100 100 5 20 edf
```

Les règles disponibles dans le Makefile sont `all`, `clean`, `fclean` et `re`.

## Politiques d'ordonnancement

Chaque dongle possède un tas de priorité contenant les requêtes en attente.

- FIFO donne la priorité à la requête arrivée en premier.
- EDF donne la priorité au codeur dont l'échéance de burnout est la plus proche.
- Lorsque plusieurs échéances EDF sont égales, elles sont départagées par
  l'ordre d'arrivée, puis par l'identifiant du codeur.

## Cas de blocage pris en charge

### Prévention des interblocages

Les codeurs demandent leurs deux dongles dans l'ordre croissant de leur
identifiant. Tous les codeurs suivent donc le même ordre de verrouillage, ce
qui supprime la condition d'attente circulaire nécessaire à un interblocage.

### Prévention de la famine

Les requêtes ne peuvent pas contourner le tas de priorité. FIFO conserve
l'ordre d'arrivée, tandis qu'EDF donne la priorité au codeur dont l'échéance de
burnout est la plus proche.

### Temps de refroidissement des dongles

Lorsqu'un dongle est libéré, l'heure de sa prochaine disponibilité est
enregistrée. La première requête attend avec `pthread_cond_timedwait` jusqu'à
la fin du refroidissement. La condition est vérifiée de nouveau après chaque
réveil.

### Détection précise du burnout

Un thread moniteur séparé vérifie le début de la dernière compilation de chaque
codeur. Il effectue ses contrôles à intervalles courts et affiche le burnout en
verrouillant les mutex de sortie et d'arrêt.

### Logs sérialisés

Chaque message est affiché pendant que le mutex de sortie est verrouillé.
L'état d'arrêt est vérifié avant l'affichage afin qu'aucun message normal
n'apparaisse après le message final de burnout.

### Cas d'un seul codeur

Avec un seul codeur, il n'existe qu'un seul dongle. Le codeur le prend, ne peut
pas en obtenir un second et finit par faire un burnout.

## Mécanismes de synchronisation des threads

Le projet utilise les mécanismes de synchronisation POSIX suivants :

- un mutex par dongle protège son propriétaire, son temps de refroidissement
  et son tas de requêtes ;
- une variable de condition par dongle met les codeurs en attente en sommeil ;
- un mutex par codeur protège l'heure de sa dernière compilation et son
  compteur de compilations ;
- un mutex de sortie empêche le mélange des lignes de logs ;
- un mutex d'arrêt protège l'état global de fin ;
- des diffusions de condition réveillent les threads en attente lorsqu'un
  dongle est libéré ou lorsque la simulation s'arrête.

Par exemple, un codeur insère une requête pendant que le mutex du dongle est
verrouillé. Il attend ensuite sur la condition du dongle. Après son réveil, il
vérifie de nouveau le tas, le propriétaire et le temps de refroidissement avant
de prendre le dongle. Le moniteur lit l'état d'un codeur uniquement lorsque le
mutex d'état de ce codeur est verrouillé.

## Ressources

- [Documentation sur la programmation des threads POSIX](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- Pages de manuel [`pthread_create(3)`](https://man7.org/linux/man-pages/man3/pthread_create.3.html) et [`pthread_join(3)`](https://man7.org/linux/man-pages/man3/pthread_join.3.html)
- Page de manuel [`pthread_mutex_lock(3)`](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- Pages de manuel [`pthread_cond_wait(3)`](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html) et [`pthread_cond_timedwait(3)`](https://man7.org/linux/man-pages/man3/pthread_cond_timedwait.3p.html)
- Page de manuel [`gettimeofday(2)`](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [Conditions de Coffman pour les interblocages](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)
- [Documentation sur les tas binaires et les files de priorité](https://en.wikipedia.org/wiki/Binary_heap)

L'IA a été utilisée pour traduire le sujet du projet, générer des cas de test de concurrence et rédiger ce README. Les suggestions et les tests générés ont été vérifiés par rapport aux exigences du sujet d'origine, compilés avec les flags obligatoires et testés avec ThreadSanitizer. L'implémentation finale ainsi que ses choix de conception doivent être compris et défendus par l'auteur.
