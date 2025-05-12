# PROJECT-IF112
IF112 - Project computer science for system compressing image using coding HUFFMAN

# Compression d'Image par Huffman

## Vue d'ensemble

Ce projet implémente l'algorithme de codage de Huffman en C pour compresser des images au format PPM (Portable Pixmap). L'objectif est d'atteindre une compression sans perte, permettant de reconstruire parfaitement l'image originale à partir des données compressées.

## Fonctionnalités

- Charger et sauvegarder des images PPM.
- Calculer l'histogramme et l'entropie de l'image.
- Construire un arbre de Huffman basé sur les fréquences d'intensité des pixels.
- Générer des codes de Huffman pour une compression efficace.
- Compresser et décompresser des images en utilisant le codage de Huffman.

## Explication du Projet

Le projet utilise l'algorithme de codage de Huffman, qui est une méthode de compression de données sans perte. Voici comment il fonctionne :

1. **Chargement de l'image** : L'image PPM est chargée en mémoire, et les données des pixels sont extraites.
2. **Calcul de l'histogramme** : Un histogramme est créé pour compter le nombre d'occurrences de chaque intensité de pixel (valeurs de 0 à 255).
3. **Calcul de l'entropie** : L'entropie de l'image est calculée pour évaluer la quantité d'information présente.
4. **Construction de l'arbre de Huffman** : Un arbre binaire est construit à partir des fréquences d'intensité, où les intensités les plus fréquentes obtiennent des codes plus courts.
5. **Génération des codes de Huffman** : Des codes binaires sont générés pour chaque intensité de pixel en parcourant l'arbre de Huffman.
6. **Compression de l'image** : Les données de l'image sont compressées en utilisant les codes de Huffman et sauvegardées dans un nouveau fichier.

## Exigences

- GCC (GNU Compiler Collection) ou tout compilateur C compatible.
- Make (optionnel, pour utiliser le Makefile).

## Installation

1. Clonez le dépôt :

   ```bash
   git clone https://github.com//huffman-image-compression.git
   cd huffman-image-compression
