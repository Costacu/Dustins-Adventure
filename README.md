# Dustin's Adventure

**Jocul este centrat în jurul lui Dustin, un personaj din serialul Stranger Things, care trebuie să scape din Laboratorul din Hawkins, 
dar laboratorul este plin de Demogorgoni, care îl vor prinde dacă nu este atent. Jucătorul va trebui să călătorească prin diferite camere 
de tip labirint și va rezolva puzzle-uri pentru a deschide ușa principală a laboratorului, în același timp în care va distrage Demogorgonii 
folosind budincă de ciocolată, preferata lui Dustin, sau se va ascunde de ei în debaralele plasate prin laborator.**

**Dar atentie! Dupa ce treci prin portal, camerele in care vei ajunge sunt oglindite fata de cele prin care ai trecut deja!
(Pentru ca esti in Upside Down) Pentru a putea iesi din Laborator, trebuie sa deschizi generatorul aflat intr-una dintre camere 
si sa activezi butoanele in ordinea corecta.**

**Hint: Starea butoanelor dintr-o camera este inversa fata de starea butoanelor din cea de-a doua camera, adica atunci cand un buton din prima camera
este ON, butonul corespondent din cea de-a doua camera va trebui sa fie OFF.**

## 🎮 Gameplay și Caracteristici

* **Explorare:** Navighează prin 4 camere.
* **Demogorgoni:** Ei patrulează coridoarele. Dacă ajungi in raza lor de detectie, te vor urmări. Dacă iesi din acea raza, 
* se vor întoarce la patrulare.
* **Debarale:** Ascunde-te în ele pentru a scăpa de Demogorgoni.
* **Distragere:** Folosește **Budinca de Ciocolată** pentru a distrage Demogorgonii.
* **Puzzle-uri:**
  * Găsește **Lopata** pentru a curăța dărâmăturile.
  * Pornește **Generatorul** pentru a alimenta sistemele.
  * Activează **Butoanele** în ordinea corectă pentru a debloca ușa finală.

## ⌨️ Controale

| Acțiune | Tastă / Mouse | Descriere |
| :--- | :---: | :--- |
| **Mișcare** | `W`, `A`, `S`, `D` | Deplasează personajul (Sus, Stânga, Jos, Dreapta) |
| **Interacțiune** | `E` | Intră/Iesi din dulap, ia lopata, curăță dărâmături, apasă butoane |
| **Budincă (Pe loc)** | `Click Stânga` | Lasă o momeală exact unde stai (pentru capcane) |
| **Budincă (Proiectil)**| `Click Dreapta` | Aruncă o momeală în direcția cursorului |
| **Resetare** | `R` | Reîncepe jocul (doar după Game Over sau victorie) |
| **Ieșire** | `ESC` | Închide jocul |






























### Tema 0

- [ ] Nume proiect (poate fi schimbat ulterior)
- [ ] Scurtă descriere a temei alese, ce v-ați propus să implementați

## Tema 1

#### Cerințe
- [ ] definirea a minim **3-4 clase** folosind compunere cu clasele definite de voi; moștenirile nu se iau în considerare aici
- [ ] constructori de inițializare cu parametri pentru fiecare clasă
- [ ] pentru o aceeași (singură) clasă: constructor de copiere, `operator=` de copiere, destructor
<!-- - [ ] pentru o altă clasă: constructor de mutare, `operator=` de mutare, destructor -->
<!-- - [ ] pentru o altă clasă: toate cele 5 funcții membru speciale -->
- [ ] `operator<<` pentru **toate** clasele pentru afișare (`std::ostream`) folosind compunere de apeluri cu `operator<<`
- [ ] cât mai multe `const` (unde este cazul) și funcții `private`
- [ ] implementarea a minim 3 funcții membru publice pentru funcționalități netriviale specifice temei alese, dintre care cel puțin 1-2 funcții mai complexe
  - nu doar citiri/afișări sau adăugat/șters elemente într-un/dintr-un vector
- [ ] scenariu de utilizare **cu sens** a claselor definite:
  - crearea de obiecte și apelarea tuturor funcțiilor membru publice în main
  - vor fi adăugate în fișierul `tastatura.txt` DOAR exemple de date de intrare de la tastatură (dacă există); dacă aveți nevoie de date din fișiere, creați alte fișiere separat
- [ ] minim 50-55% din codul propriu să fie C++, `.gitattributes` configurat corect
- [ ] tag de `git`: de exemplu `v0.1`
- [ ] serviciu de integrare continuă (CI) cu **toate bifele**; exemplu: GitHub Actions
- [ ] code review #1 2 proiecte

## Tema 2

#### Cerințe
- [ ] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [ ] moșteniri:
  - minim o clasă de bază și **3 clase derivate** din aceeași ierarhie
  - ierarhia trebuie să fie cu bază proprie, nu derivată dintr-o clasă predefinită
  - [ ] funcții virtuale (pure) apelate prin pointeri de bază din clasa care conține atributul de tip pointer de bază
    - minim o funcție virtuală va fi **specifică temei** (i.e. nu simple citiri/afișări sau preluate din biblioteci i.e. draw/update/render)
    - constructori virtuali (clone): sunt necesari, dar nu se consideră funcții specifice temei
    - afișare virtuală, interfață non-virtuală
  - [ ] apelarea constructorului din clasa de bază din constructori din derivate
  - [ ] clasă cu atribut de tip pointer la o clasă de bază cu derivate; aici apelați funcțiile virtuale prin pointer de bază, eventual prin interfața non-virtuală din bază
    - [ ] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
    - [ ] `dynamic_cast`/`std::dynamic_pointer_cast` pentru downcast cu sens
    - [ ] smart pointers (recomandat, opțional)
- [ ] excepții
  - [ ] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim **3** clase pentru erori specifice distincte
    - clasele de excepții trebuie să trateze categorii de erori distincte (exemplu de erori echivalente: citire fișiere cu diverse extensii)
  - [ ] utilizare cu sens: de exemplu, `throw` în constructor (sau funcție care întoarce un obiect), `try`/`catch` în `main`
  - această ierarhie va fi complet independentă de ierarhia cu funcții virtuale
- [ ] funcții și atribute `static`
- [ ] STL
- [ ] cât mai multe `const`
- [ ] funcții *de nivel înalt*, de eliminat cât mai mulți getters/setters/funcții low-level
- [ ] minim 75-80% din codul propriu să fie C++
- [ ] la sfârșit: commit separat cu adăugarea unei noi clase derivate fără a modifica restul codului, **pe lângă cele 3 derivate deja adăugate** din aceeași ierarhie
  - noua derivată nu poate fi una existentă care a fost ștearsă și adăugată din nou
  - noua derivată va fi integrată în codul existent (adică va fi folosită, nu adăugată doar ca să fie)
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.2`
- [ ] code review #2 2 proiecte

## Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o clasă șablon cu sens; minim **2 instanțieri**
  - [ ] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
- [ ] minim 85% din codul propriu să fie C++
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.3` sau `v1.0`
- [ ] code review #3 2 proiecte