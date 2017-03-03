set title "AG para knapsack"
set xlabel "Geração"
set ylabel "Aptidão"

set style data lines 
plot "dados2.dat" using 1:3 title "media" with lines lt 2, "dados2.dat" using 1:2 title "melhor" with lines lt 1
pause -1 "Enter para continuar"
