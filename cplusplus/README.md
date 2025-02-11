# star-upcDst template

![Bilby Stampede](https://cds.cern.ch/record/2288105/files/fig1.png)

## Overview:
repository contains a template of an analysis using *star-upcDst*, a framework to simplify analysis that are related to forward and UPC physics. 

- Make a clean area on RACF and checkout the repository:

<pre><code> git clone https://github.com/TruhlarT/workshopPartII.git </pre></code>

- Go to the main directory workshopPartII:

<pre><code> cd workshopPartII </pre></code>

- Setup the StRoot and build ( already include compiling ) by doing:

<pre><code> ./build-maker.sh </pre></code>

In build directory, create executable file of the analysis template "Ana",

<pre><code> mkdir build </code></pre>
<pre><code> cd build </code></pre>
<pre><code> cmake ../ </code></pre>
<pre><code> make </code></pre>

## work directory:
<pre><code> cd ../work </code></pre>
<pre><code> cmake .</code></pre>
<pre><code> make </code></pre>


## Contact:
Tomas Truhlar: <Tomas.Truhlar@fjfi.cvut.cz>