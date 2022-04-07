---
layout: page
title: Assignments
permalink: /assignments/
---

<h1> Programming Assignments </h1>

<p>
We will do <font color="#FF0000">five</font> programming
assignments. Each programming assignment takes up 10% of your total grade.
Some assignments provide extra credit beyond your total grade.
</p>

<ul>
<li> Assignment 1: <a href="assignment1/index.html"> A Word Counting Program</a> (due: 03/18 11:55pm) </li>
<li> Assignment 2: <a href="assignment2/index.html"> String Manipulation</a>  (due: 04/08 11:55pm) </li>
<li> Assignment 3: <a href="assignment3/index.html"> Customer Management Table </a> (due: 04/29 11:55pm) </li>
<li> Assignment 4: (TBA)
<!-- <a href="assignment4/index.html"> Assembly Language Programming</a>  -->
(due: 05/20 11:55pm) </li>
<li> Assignment 5: (TBA)
<!-- <a href="assignment5/index.html">Unix Shell Assignment</a>  -->
(due: 06/10 11:55pm) </li>
</ul>


<h2> Assignment Submission (Important) </h2>
<hr>

<p>
Use the KLMS to submit your
assignments. (Link will be announced at KLMS.) Your submission should be one gzipped tar file whose name is <b>YourStudentID_assign#.tar.gz</b>.

For example, if your student ID is 20221234, and it is for assignment #1,
please name the file as 20221234_assign1.tar.gz.

</p><p>
To create the .tar.gz, first move all your files to the directory (20221234_assign1).
</p>

<p>
<pre class="ui message">
mkdir 20221234_assign1
mv all_your_files 20221234_assign1
</pre>

</p><p>
Then, move to your directory 20221234

</p>
<pre class="ui message">
cd 20221234_assign1
</pre>

<p>
Create a .tar.gz file by the 'tar' command like

</p><p>
<pre class="ui message">
tar -zcf 20221234_assign1.tar.gz *
</pre>

</p><p>
Then, you'll see  20221234_assign1.tar.gz. If you want to decompress and release the files in it (in a different directory),

</p>
<pre class="ui message">
tar zxf 20221234_assign1.tar.gz
</pre>

<h2> Ethics Document (Important) </h2>
<hr>

<p>
For every assignment submission, please fill out and submit
the <b>pdf</b> version of <a href="../assignments/EthicsOath.docx">this
document</a> that pledges your honor that you did not violate any
ethics rules required by <a href="../policy">this course</a> and
KAIST.  You can either scan a printed version into a pdf file or make
the Word document into a pdf file after filling it out.

</p><p>
Please sign on the document and submit it along with your other
assignment files, or we won't grade your assignment.


</p><h2> Late Submission </h2>
<hr>

<p><b>No late submission is allowed!</b></p>

<h2> Collaboration Policy </h2>
<hr>

<p>
Please refer to <a href="../policy">the course policy page</a>.


</p><h2> Coding Style </h2>
<hr>

<p>
Good coding style will be one criterion for grading each
assignment. Please make sure your code has proper indentation and
descriptive comments. At the start of each file, please add your name,
lab account ID and the description of the file. Make sure not to leak
any memory and check/handle every return value of function calls.
</p>

<h2> Assignment Grading </h2>
<hr>

<p>
Your submission will be graded on one of the Lab machines (eelab5) for the
course. You are free to use other machines for coding and debugging,
but please make sure to compile and test your final version on the Lab
machines (eelab5). In a rare case, library mismatch or O/S stack difference
(Solaris vs. Linux) can bypass some of your bugs, but they can
actually show up on the Lab machines while grading. In order to avoid
this last-minute surprise, please test on Lab machines before
submitting your work.
</p>
<p>We will use an automated program for scoring.
And it works similar below practice in assignments.
So you can check your functionality using diff command.</p>

<div class="ui message" style="margin-left:1em;"><pre>./samplewc209 &lt; <em>somefile</em> &gt; output1 2&gt; errors1
./wc209 &lt; <em>somefile</em> &gt; output2 2&gt; errors2
diff output1 output2
diff errors1 errors2
rm output1 errors1 output2 errors2</pre>
</div> 

<!--
<h2> Manual grading policy </h2>
<hr>

<p>
Trivial mistake in your submission may cause huge amount of deduction in your assignment score.
For such exceptional circumstances, you can request and get manual grading at the TA's discretion.
This is only allowed for <b>assignment 1</b>,
and you will get <font color="#FF0000">15% amount of deduction</font> from your total score.
</p>
<br>
-->
<hr>

<script src="{{ "/vendor/moment.min.js" | relative_url }}"></script>
<script src="{{ "/vendor/ee209_assignment.js" | relative_url }}"></script>
