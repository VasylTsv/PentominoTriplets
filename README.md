# PentominoTriplets
Complete solver for pentominoes triplets puzzle

One of my very old projects, it has been more than 16 years since I've done it. Pentomino puzzles were among my favorites ever since I've read S. Golomb's book. The pentominoes triplet problem is very well known - create three identical shapes with a complete set of 12 pentominoes - but I haven't seen a references to a complete solution. So, I've tried to solve it. The file is called `supersolver.cpp` because it was preceded by a number of special case solvers. Eventually I've figured out how to enumerate all possible shapes. As I remember, it took close to 24 hours on then mid-range Athlon. Haven't tried it lately, but it should still take a few hours.

If I did this today, I would probably try DLX instead of straightforward backtracking, although it is not clear if the speed advantage of DLX would not be offset by the setup complexity. I would also try to run solver on multiple threads and farm solving from the enumerator thread. That seems to be completely doable but the scheduling may be a challenge. I don't expect solvers to overrun the puzzle generators and starve but the generator may outrun solvers significantly and create the queue that will take more memory. So, some experimentation is necessary.

The result was posted on Ed Pegg Jr.'s MathPuzzle.com. Apparently, somebody beat me to the complete solution (which is not surprising), but I haven't seen an explanation  how that other solution was found.

The code may be a little rough, and it may run into small issues with modern compilers. It is presented here in the exact form I've got it all those years ago.

The algorithm to enumerate all possible shapes is taken from [this page](http://kevingong.com/Polyominoes/ParallelPoly.html), which is amazingly still up.
