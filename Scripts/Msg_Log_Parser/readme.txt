1. Create a folder to hold the sim.converter.jar

2. Inside the folder create 2 sub folders: input, output

3. Put the initial scenario file (.json) and pandemic_messages.txt inside the input folder
	3.1. The converter will look for a file with "_messages" in the file name, must be somewhere.

4. In the command line navigate to the folder holding the sim.converter

5. Run the following command: java -jar sim.converter.jar ".\\input\\" ".\\output\\"

6. Your results should now be in a .zip file in the output folder.

