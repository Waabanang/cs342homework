# How to build the Linux Kernel

Buidling the linux kernel is a simple process involving [number] discrete steps and lots of waiting

# Get the source

The first step is getting the source code for the kernel. An easy way to do this is to simple clone the upstream.
<pre><code>git clone git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git</code></pre>

# Making with Make

From here the Makefile is your best friend. It's a terribly terribly complicated Makefile, so I won't go into detail on what it's doing, except to mention that I changed where I changed it. In order for my 'version' of the kernal to appear unique, I added had EXTRAVERSION="waabafet"

First you `make` the config:

<pre><code>make oldconfig</code></pre>

Then the image:

<pre><code>make bzImage</code></pre>

Then the modules:

<pre><code>make modules</code></pre>

Having successfully built all the parts, we now need to install them which is again handled by the Makefile and `make`. This needs to be done as root.

<pre><code>make modules_install</code></pre>

<pre><code>make install</code></pre>

And now you've built the linux kernal, you can boot into it from your Grand Unified Bootloader. 