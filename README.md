# Prisoners Problem (Using Multi-Threading)


 Program solvers prisoners problem using two strategies.

The director of a prison offers 100 death row prisoners, who are numbered from 1
to 100, a last chance. A room contains a cupboard with 100 drawers. The director
randomly puts one prisoner’s number in each closed drawer. The prisoners enter the
room, one after another. Each prisoner may open and look into 50 drawers in any
order. The drawers are closed again afterwards. If, during this search, every prisoner
finds his number in one of the drawers, all prisoners are pardoned. If just one prisoner
does not find his number, all prisoners die. Before the first prisoner enters the room,
the prisoners may discuss strategy — but may not communicate once the first prisoner
enters to look in the drawers. What is the prisoners’ best strategy?

1) 
A simple strategy is that every prisoner randomly chooses drawers while searching for his number.
This strategy is not very effective since every prisoner has a 50% chance to find his number. This
means that 100 prisoners have a chance of 0.5
100 to all find their numbers, which is almost zero.

2) 
A smart strategy is that every prisoner starts by opening the drawer with his own number. If the
number in the drawer matches, he has been successful. If not, he next opens the drawer with
the number found in the current drawer, i.e., the prisoner follows a pre-determined sequence of
drawers to find his number. This strategy provides a surprisingly high chance for all 100 prisoners
to find their numbers.


program initializes 100
drawers and 100 prisoners and then runs different game strategies (at least random and smart).
Implementing the prisoners as concurrent threads. In order to open a drawer, a thread (prisoner) first
has to acquire a lock. 

Using a global lock for all drawers: A prisoner first obtains the global lock and then executes
the search for his number. When done, the prisoner releases the global lock. In other words,
prisoners execute their searches sequentially.



## how to compile



```bash
gcc prisoners.c -o prisoners
./perfect 
```


## Usage
Default number games 100.
The command line option -n determines how many games to simulate. The default is 100 games.
The command line option -s seeds the random number generator with a non-static value. An
example execution might produce the following output:
```terminal
./prisoners -n 100 -s 
```


```terminal
./prisoner
method random_global 0/100 wins = 0.00% 191.161 ms
method strategy_global 26/100 wins = 26.00% 219.138 ms

```