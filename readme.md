# Qt Version

_This version uses Qt5 `mappedReduced()` function._

_This means that:_
- _There is no upper bound on the queue._
- _We read everything and ONLY THEN we unzip, index and merge. It's bad..._
****
Better representations might be:
- `QtFutureWatcher` could not think of a proper solution here.
- _I could not find the source code of `mappedReduced`, but if I could see the way how the data is extracted from `QVector` or a similar data type, there is a possibility of writing a custom wrapper over QVector which would limit itself. But that is very dependent on the `mappedReduced` implementation._
****
### Results

_The sanitizers are mad over a data race, which I could not locate, may be a false positive since by **testing multiple times the results are the same with the previous lab**._

_Directory **/3** of the smaller archive_ (like 3GB or something.)
`res_a.txt`: https://drive.google.com/file/d/1XoSN5r8hOW1g6f1SzN6WYfv82XPqxrNR/view?usp=sharing
`res_n.txt`: https://drive.google.com/file/d/1WUVLL3v5OUAmIPTH-36LgUzD0mEmv8fL/view?usp=sharing