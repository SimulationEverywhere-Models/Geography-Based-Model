
Info about Statistics Canada census boundaries
	- https://www150.statcan.gc.ca/n1/pub/92-196-x/92-196-x2016001-eng.htm
	- https://www12.statcan.gc.ca/census-recensement/2016/ref/dict/az1-eng.cfm


	Dissemination Area

	A dissemination area (DA) is a small, relatively stable geographic unit composed of one or more adjacent dissemination blocks with 
	an average population of 400 to 700 persons based on data from the previous Census of Population Program. It is the smallest standard 
	geographic area for which all census data are disseminated. DAs cover all the territory of Canada.

	Dissemination Block

	A dissemination block (DB) is an area bounded on all sides by roads and/or boundaries of standard geographic areas. The dissemination 
	block is the smallest geographic area for which population and dwelling counts are disseminated. Dissemination blocks cover all the 
	territory of Canada.

------------------------------------------------------------------------------------------------------------------------------------------------

Info about attributes 
	https://www150.statcan.gc.ca/n1/pub/92-150-g/92-150-g2016002-eng.htm

	uid		Unique identifier
	tdwell		Total private dwellings
	urdwell		Private dwellings occupied by usual residents
	area		Land area
	ir		Incompletely enumerated Indian reserve or Indian settlement
	_2016		Represents 2016 data

------------------------------------------------------------------------------------------------------------------------------------------------
	
There are 3 datasets in the archive:
	DB Ottawa	: All dissemination blocks for Ottawa		+- 8000 records
	DA Ottawa	: All dissemination areas for Ottawa		+- 1400 records
	DA Ontario	: All dissemination areas for Ontario		+- 20000 records

Each dataset has 3 files:
	* Clean.csv 	: Contains the census geography joined with attribute data (pop count, area, number of dwelling and a couple more)
	* Adjacency.csv	: Contains the adjacency for the geographies in the Clean file. Each geography is related to each geography it touches. 	
			  by its boundary. The result is a 1 to N relationship.
	*.png		: A screenshot of the geographic area, for reference. 