---
layout: page
title: Assignments
permalink: /assignments/
---

<h1> Programming Assignments </h1>

<p>
We will do <font color="#FF0000">five</font> programming
assignments. Each programming assignment takes up 10% of your total grade.
</p>

<ul>
<li> Assignment 1:
A Word Counting Program</a> (TBD) </li>
<li> Assignment 2:
String Manipulation</a> (TBD) </li>
<li> Assignment 3:
 Customer Management Table</a> (TBD) </li>
<li> Assignment 4:
Assembly Language Programming</a> (TBD) </li>
<li> Assignment 5:
Unix Shell Assignment </a> (TBD) </li>
</ul>


<h2> Grading Policy of Assignments </h2>
<hr>
<table style="border-collapse: collapse;" border="3" cellpadding="3">
<tbody>

<tr>
<td> 10% </td>
<td> Assignment #1  </td>
</tr>

<tr>
<td> 10% </td>
<td> Assignment #2  </td>
</tr>

<tr>
<td> 10% </td>
<td> Assignment #3  </td>
</tr>

<tr>
<td> 10% </td>
<td> Assignment #4  </td>
</tr>

<tr>
<td> 15% </td>
<td> Assignment #5  </td>
</tr>

</tbody>
</table>


<h2> Assignment Submission (Important) </h2>
<hr>

<p>
Use the KLMS to submit your
assignments. (Link will be announced at KLMS.) Your submission should be one gzipped tar file whose name is <b>YourStudentID_assign#.tar.gz</b>.

For example, if your student ID is 20231234, and it is for assignment #1,
please name the file as 20231234_assign1.tar.gz.

</p><p>
To create the .tar.gz, first move all your files to the directory (20231234_assign1).
</p>

<p>
<pre class="ui message">
mkdir 20231234_assign1
mv all_your_files 20231234_assign1
</pre>

<p>
Create a .tar.gz file by the 'tar' command like

</p><p>
<pre class="ui message">
tar zcf 20231234_assign1.tar.gz 20231234_assign1
</pre>

</p><p>
Then, you'll see 20231234_assign1.tar.gz. If you want to decompress and release the files in it (in a different directory),

</p>
<pre class="ui message">
tar zxf 20231234_assign1.tar.gz
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
<p>
Students can use two late submission tokens which can be late up
to <i>one day</i> with penalty for the programming assignments.
That is, you can use your tokens for a total of two assignments.
Your token will be used automatically, i.e., the token is applied
to the first two assignments you submit late.

With your tokens, you can submit your assignment with the following penalty.
</p><ul>
<li> 95% of the full credit up to 1 hour late,
</li><li> 80% of the full credit up to 24 hours late,
</li><li>  0% of the full credit beyond 24 hours late.
</li></ul>

<p> <b>Important</b>:
<ul>
<li> Any late submission without token will result in zero credit.
</li><li> Note that we do not accept late submission for the <b>last
assignment (assignment 5)</b>.
</li></ul>
</p>

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
student ID, and the description of the file. Make sure not to leak
any memory and check/handle every return value of function calls.
</p>

<h2> Assignment Grading </h2>
<hr>

<p>
Your submission will be graded on one of the Lab machines (eelabg1 or eelabg2) for the
course. You are free to use other machines for coding and debugging,
but please make sure to compile and test your final version on the Lab
machines (eelabg1 or eelabg2). In a rare case, library mismatch or O/S stack difference
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

<h2> Fix a small mistake </h2>
<hr>

<p>
Because we used an automatic grading system, trivial mistake in your submission
may cause huge amount of deduction in your assignment score.  For such
exceptional circumstances, you can request changes and get modified grading at
the TA's discretion.  You will get <font color="#FF0000">15% amount of
deduction</font> from your total score. Note that once submitted no alogirhtm change to the code is allowed and you cannot propose any changes to the submitted code.
</p>

<h2> Extra point </h2>
<hr>
<p>
Some programming assignments provide extra points. These extra points cannot exceed
the total score (50% of final score) that you can obtain from assignments. But, within the total score,
the extra points will affect your final score.
</p>

<h2> gcc209 </h2>
<hr>
<p>
gcc209 is a special script made for EE209. Students need to use the script for assignments. How to make gcc209 is as follows.
<br>1. Make the following script by yourself using editor
<div class="ui message" style="margin-left:1em;">
<pre>
#!/bin/bash
gcc -Wall -Werror -ansi -pedantic -std=c99 "$@"
</pre>
</div>
</p><p>2. Make this script executable.
<div class="ui message" style="margin-left:1em;">
<pre>
&#36;chmod +x gcc209
</pre>
</div>
</p><p>3. Move this file to folder that can be accessed globally
<div class="ui message" style="margin-left:1em;">
<pre>
&#36;sudo mv gcc209 /usr/bin/gcc209
</pre>
</div>
</p>
<script src="{{ "/vendor/moment.min.js" | relative_url }}"></script>
<script src="{{ "/vendor/ee209_assignment.js" | relative_url }}"></script>
