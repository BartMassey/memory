set terminal pdf
set output "memory.pdf"
set title "Memory Size vs Access Time"
set xlabel "Size (log2 words)"
set ylabel "Time (log2 seconds)"
plot for [ix=0:5] 'memory.dat' index ix using 1:2 with lines title columnhead(1)
