# Exercice — Les trois cadences

a 72 Hz, une image dure 13,9 ms, a 90 Hz, une image dure 11,1 ms, a 120 Hz, une image dure 8,3 ms.(periode = 1000 / fréquence )

Si on enlève les 8 ms fixes prises par les capteurs, la transmission, la composition et l'affichage, il reste au code :

- pour 72 Hz, 13,9 - 8 = 5,9 ms
- pour 90 Hz, 11,1 - 8 = 3,1 ms
- pour 120 Hz, 8,3 - 8 = 0,3 ms

Plus la cadence monte, moins le code a de marge.