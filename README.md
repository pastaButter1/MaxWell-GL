# MaxWell-GL
## L'Idée
Au début de la session nous avions le goût de construire nous même un projet incorporant des composantes graphiques et des notions scientifiques. C'est de cette manière que nous avons eu l'idée de simuler le champ magnétique de composantes électroniques. Notre objectif était de créer une application interactive en 3 dimensions avec le moins de dépendances possible qui permettraient à des étudians et à des professeur d'explorer les propriétés pratiquement magiques de l'électromagnétisme. Rapidement, nous avons bati un prototype avec trois fils parallèles. Les flèches indiquent la direction du champ alors que l'intensité du rouge est liée à l'intensité du champ magnétisme.

<img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/Prototype.png" width="400" height="400" />

## Le projet
### Visual Studio
MaxWell-GL a été dévoloppé et bati en tant que solution Visual Studio 2019.

### C++
Nous avions à coeur d'écrire le projet en C++. Ce language nous donne la liberté de créer les structures et les paradignes de programation qui s'appliquent le plus à notre projet tout en gardant des qualité de vie et la possibilité d'utiliser certaines caractéristiques de la programation orienté objet enforcée par des languages comme Java. 

### OpenGL
Nous avons utilisé l'API [OpenGL](https://www.khronos.org/opengl/) comme outils pour interfacer avec l'écran, mais aussi comme outil pour nous permettre de pleinement utiliser la carte graphique et de drastiquement accélérer les calculs devant être fait. Par nature, la carte graphique est composé de centaines voir de millier d'unités de calculs capable de paralléliser des calculs matématiques. 

Contrairement à la programation fait sur un processeur central (CPU), la programmation sur une carte graphique (GPU) est bien différente. Les programmes ne sont pas éxécutés de manière linéaire. Au contraire, ils sont fait pour être les parallèles possible. Aussi, les ressources et les données de la carte graphique ne peuvent être invoquées au milieu d'un programme comme on peux le faire sur le processeur. On doit, depuis de le processeur, lier certains états ou plus simplement, ordonner un chemin d'exécution au programme, et lier les données qui seront utilisés par des *shaders*. Ainsi, il faut présenter les dfférents *shaders* en plus des données qui seront utlisés avant l'éxécution. Une fois l'éxécution terminés, les données sont retournées à l'endroit spécifié où le programme peut les récupérer. Cette architecture s'apparante à celle d'un pipeline.

![Exemple de pipeline](https://vulkan-tutorial.com/images/vulkan_simplified_pipeline.svg)
[source](https://vulkan-tutorial.com/images/vulkan_simplified_pipeline.svg)

#### GLEW
Puisque OpenGL n'est qu'une spécification garantie par les concepteurs et vendeurs de carte graphique, ils ne sont oubligés que de fournir les ressources attendues selon le manuel d'instruction. Or, la carte graphique n'est pas directement liée au processeur central. Elle est plutôt liée par une interface PCIe qui nécessite des appels au syystème d'opération que cela soit Windows, Linux ou MacOS. Ainsi, les  appels de fonctions peuvent varier selon la plateforme. De ce fait, des librairies sont nécessaire pour à la fois redéfinir les appels de fonctions pour le language de programation spécifique et redéfinir les appels de fonctions pour qu'ils soient facilement compréhensible pour l'utilisateur. C'est pour cela que nous avons décidés d'utiliser l'interface [GLEW](https://glew.sourceforge.net/) dans notre projet.

#### GLFW
Encore, l'utilisation d'API d'aussi bas niveau que OpenGL par rapport à des moteurs graphique comme Unity ou Unreal Engine implique qu'il est du devoir de l'utilisateur de créer son environnement graphique ce qui inclus la fênetre et tous les appels de systèmes qui sont nécessaire pour enregistrer les entrées de la souris ou du clavier de l'utilisateur. Ainsi, nous avons choisi d'utiliser [GLFW](https://www.glfw.org/), une librairie libre de droits pour gérer ces composantes du projet qui nous auraient pris beaucoup de temps et qui nous auraient empêché d'accomplir nos objectifs.

#### GLM
Enfin, nous avons eu besoin d'une dernière libraire: [GLM](https://github.com/g-truc/glm). C'est une librairie en C++, basée sur les *templates* ou *generics* dans d'autres languages ainsi que la surcharge d'opérateur, qui contient énormément de définitions de types mathématiques que cela soit de vecteurs ou matrices ainsi que des fonctions connexes qui facilitent grandement la programation dans un environnement 3D Elle présente aussi les données dans un format qui est directement compatibles avec les API graphiques comme OpenGL que nous utilisons.

### Les calculs physiques
Pour calculer le champ magnétique dans noter simulation, nous divisons l'espace au grand complet en petits cubes appelés *voxels* qui contiennent la valeur du champ magnétique à cet endroit en particulier. Aussi, les cartes graphiques sont conçues avec énormément de coeurs capables de faire énormément de calculs, mais peu de logique ce qui les rendent très utiles afin d'accélérer des calculs. Justement, le champ dans chaque voxel est calculé indépendament de celui de son voisin. De ce fait, nous pouvons séparer les dépendances enter les calculs de la même manière que nous avons séparés le stockage, soit dans un espace en trois dimensions. C'est pour cela que les *compute shader* sont utiles. Il est possible d'invoquer les tâches dans les trois dimensions en même temps en ajustant le nombre de *work group* dans chacune des dimensions comme dans le diagram ci-dessous. Chacun des cubes représente une tâche à effectuer. La carte graphique effectuera le compute shader pour chacun d'eux.
Enfin, une fois les calculs finis, les flèches disposés sur le plan de coupe interpolent les données qui indiquent leur magnitude et leur direction.

![Shéma work group](https://learnopengl.com/img/guest/2022/compute_shader/local_space.png)
[source](https://learnopengl.com/img/guest/2022/compute_shader/local_space.png)

### Les équations
À ce stade du projet, seule l'équation de Biot-Savard a été utilisé pour calculer le champ magnétique autour d'un fil ou d'un solénoïde.

$$ \vec{B} = {\mu_0 i \over 4\pi} \int {{{\mathrm d \vec{l}} \times \vec{u}} \over r^2} $$

Dans le cas d'un fil droit présumablement infini, le champ est toujours perpendiculaire à la direction du fil et il est possible de simplifier l'équation en enlevant l'intégrale. Il est donc possible de seulement calculer le champ à partir du vecteur orthogonal reliant le point de mesure au fil. L'équation est refaite pour tous les fils présents dans la simulation.

$$ \vec{B} = { \mu_0 i \over 2 \pi r }  ( \vec{u_l} \times \vec{u_{r\perp}} ) $$

Dans le cas d'un solénoïde, dont la longueur, le nombre de tours et le rayon sont arbitraires, nous avons décidés d'approximer l'intégrale en intégrant le champ d'après $\theta$. Ainsi nous obtenons l'intégrale définie qui peut être approximée comme ci-dessous. Le champ est calculé pour chacun des voxels de l'espace vectoriel.

$$ \vec{B} = {\mu_0 i \over 2 \pi} \int\limits_0^{2 \pi L \over N} {{{\mathrm d \vec{\theta}} \times \vec{u}} \over r^2} $$

### ImGUI
Afin de construire l'interface graphique du projet, nous avons utilisé la librairie [ImGUI](https://github.com/ocornut/imgui). Elle est extrêmement populaire dans l'industrie du jeux-vidéo en plus d'être supportée par la plupart des moteurs graphiques utilisés. Elle partage des similitudes avec JavaFx par sa simplicité. Nous l'avons donc utiliser pour offrir un outil à l'utilisateur afin qu'il puisse modifier et jouer avec son environnement. Bien qu'incomplet, certains paramètres sont déjà accessibles et permettent de modifier les dimensions du solénoïde, de déplacer le plan de coupe et de modifier certaines propriétés graphiques et esthétiques de la scène.

##### Voici une démonstration de l'éxécutable à ce moment:
[![Watch the video](https://img.youtube.com/vi/CFjZAVqjIHM/hqdefault.jpg)](https://www.youtube.com/embed/CFjZAVqjIHM)


## Les difficultés
Bien que nous avions de l'expérience avec les outils que nous avons utilisés, nous avons tout de même fait face à des difficultés. La majorité étaient en lien direct avec OpenGL. Des outils de déboguage sont disponible. [RenderDoc](https://renderdoc.org/) est libre de droits. Nous l'avons utilisé. Voici quelques captures d'écran que nous avons prises.

|       |       |
| :---: | :---: |
| Application <img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/Capture d’écran 2025-05-22 221212.png" width="500" height="300" /> | Texture <img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/Capture d’écran 2025-05-22 221321.png" width="500" height="300" /> |
| Pipeline <img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/Capture d’écran 2025-05-22 221417.png" width="500" height="300" /> | Mesh <img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/Capture d’écran 2025-05-22 221442.png" width="500" height="300" /> |

Toutefois, certaines erreurs sont complexes. Par exemple, si un buffer n'est pas lié correctement, le débogueur n'affichera aucune erreur. Il nous informera seulement qu'il n'a pas trouvé la ressource. Si une erreur arrive, l'application pourrait tout simplement se fermer sans laisser la chance au programmeur de l'inspecter. Certaines erreurs peuvent être surnoises dans le cas où un type n'accepte que des valeurs bien spécifiques. La lecture de la documentation devient une étape essentielle. C'est donc au programmeur de devoir déduire la provenance des erreurs. Des techniques de déboguages où on retire peu à peu du code jusqu'à ce qu'on retrouve un projet fonctionnel deviennent extrèmement utiles.

Même avec ces précautions, certains bogues sont tout simplement impossible à corriger dans le programme parce qu'il ne provienne pas de lui. Une fois, il m'est arrivé que les drivers de ma carte graphique causent des erreurs. J'ai perdu quelques jours à tenter de résoudre le problème avant de penser à les mettre à jour. Le problème est parti à cet instant.

Du même coup, implémenter des fonctionnalités qu'aucun de nous deux n'a déjà utilisé devient compliqué. La documentation donne juste assez d'information pour s'en sortir. Cela nous oublige à fouiller sur divers forums et à perdre énormément de temps à comprendre toutes les spécificités d'une fonctionnalité.

## L'échéancier
Bien que nous n'ayons pas pu paufiner le projet comme nous le voulions, nous avons tout de même atteints la majorité de nos objectifs. Certaines parties de l'échéancier ont pris un peu de retard en cours de chemin: principalement l'interface,  fonctionnelle mais incomplète, et la simulation. Ce retard est dû aux diffultés qui ont étés rencontrés lorsque je [Charles-Antoine] ai essayé d'ajouter mon code à celui de mon frère. Il a parfois été nécessaire de réécrire des parties entières. Toutefois, les simulations des solénoïdes et des fils sont présentes et fonctionnelles dans le projet, il est seulement arrivé que nous n'avons pas eu la détermination afin d'ajuster la fonction qui crée le solénoïde pour qu'elle crée un fil à la place. Ainsi, sans modèle et interface pour facilement aller d'un à l'autre, nous n'avons inclus que le solénoïde.

<img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/Capture d’écran 2025-05-22 220136.png" width="1000" height="400" />


## Architecture
Étant donné que notre projet fait usage du processeur central et de la carte graphique, nous avons dû nous assurer de bien scinder les différentes parties du projet de manière à limiter les dépendances et les erreurs. Pour notre projet, nous avons fait le flux de données ci-dessous. Le programme peut être diviser en trois parties distinctes. La première, le processeur, s'occupe de la logique du projet ainsi que la gestion des deux autres parties du programme. La deuxième, la carte graphique, fait des calculs mathématiques et graphiques intenses et parallélisables alors que la troisième partie, l'interface, recueille les entrées de l'utilisateur et les relaie au processeur centrale. La porté des fonctions présentes dans le projet se limite à leur partie. Si elles doivent communiquer avec une autre partie, elles font appels à d'autres fonctions où le code est isolé. 

<img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/Diagramme intégration.png" width="400" height="400" />

Pour ce projet, nous n'avons pas formellement utilisé de programmation orientée objet. Cela implique que nous n'avons pas utilisé de polyformisme ni d'héritage. Les données sont plutôt assemblés en structures dans lesquelles sont aussi définies des fonctions statiques. Ces fonctions statiques prennent en paramètre la structure dont elles font partie. Ces définitions peuvent s'apparenter à des namespaces. Le graphique ci-dessous, fait par l'outil de documentation [Doxygen](https://www.doxygen.nl/), indique les liens de dépendance entre nos structures. Comme il est possible de voir, nous nous sommes assurés d'avoir une hiérarchie entre les fonctions. Certaines fonctions appartenant aux moteurs graphiques et physiques ne peuvent être appelés que par des fonctions plus hautes. Cela évite les dépendances circulaires.

<img src="https://github.com/pastaButter1/MaxWell-GL/blob/main/RessourceReadme/struct_application__coll__graph.png" width="900" height="400" />


## Les perspectives
Le projet, bien qu'à un stade fonctionnel, est encore loin d'être fini. Nous aurions aimé avoir été capable d'intégrer plus de fonctionnalités dans l'application. Il aurait été intéressant de d'avoir plus de composantes électroniques comme des fils, des toroïdes, etc. Il aurait aussi été plaisant d'intégrer des simulations dans le temps où le champ magnétique pourait varier selon un courant défini. L'utlisateur aurait ainsi pu étudier en plus grande profondeur les mécaniques du courant alternatif. Avant tout, il faudrait aussi revoir l'interface utilisateur afin de réduire le plus possible tout information écrite statiquement dans le code et afin de permettre à l'utilisateur de contrôler son environnement le plus possible. Il devrait être capable d'ajouter ou de retirer des composantes, de modifier la position et l'orientation de celles-ci, de modifier leurs propriétés électriques ou graphiques, etc.

<img src="https://i5.walmartimages.com/asr/673fcd95-2bfd-405d-88f6-0744d2af719e.6d74a8f705166fa7dfd0bf0512c75b7a.jpeg?odnHeight=612&odnWidth=612&odnBg=FFFFFF" width="400" height="400" />

[source](https://i5.walmartimages.com/asr/673fcd95-2bfd-405d-88f6-0744d2af719e.6d74a8f705166fa7dfd0bf0512c75b7a.jpeg?odnHeight=612&odnWidth=612&odnBg=FFFFFF)


## La conclusion
En conclusion, notre planification était plus ambitieuse que ce que l'on a pu réaliser. Nous avons tout de même atteins certains buts. Nous avons une application OpenGL fonctionnelle qui permet tout de même d'en apprendre plus sur l'électromagnétisme.


## Crédit
Le projet a été codéveloppé dans le cours d'intégration en sciences informatiques et mathématiques au Collège Bois-de-Boulogne par Charles-Antoineet Alexandre Lafrenière.


## Licence
Le projet est libre de droits sous la licence GNU. Voir [GPL-3.0 licence.txt](https://github.com/pastaButter1/MaxWell-GL?tab=GPL-3.0-1-ov-file#) pour plus d'information.
