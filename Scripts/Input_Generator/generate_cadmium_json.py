# Original author: Kevin
# (Slightly) modified by: Binyamin

#!/usr/bin/env python
# coding: utf-8

# In[14]:


import pandas as pd
import geopandas as gpd
from collections import defaultdict, OrderedDict
import json


# In[15]:


def shared_boundaries(gdf, id1, id2):
    g1 = gdf[gdf["dauid"] == str(id1)].geometry.iloc[0]
    g2 = gdf[gdf["dauid"] == str(id2)].geometry.iloc[0]
    return g1.length, g2.length, g1.boundary.intersection(g2.boundary).length

def get_boundary_length(gdf, id1):
    g1 = gdf[gdf["dauid"] == str(id1)].geometry.iloc[0]
    return g1.boundary.length


# In[16]:


df = pd.read_csv("/home/binybrion/Documents/cadmiumgis/data/DA Ottawa Clean.csv")  # General information (id, population, area...)
df_adj = pd.read_csv("/home/binybrion/Documents/cadmiumgis/data/DA Ottawa Adjacency.csv")  # Pair of adjacent territories
gdf_ontario = gpd.read_file("/home/binybrion/Documents/cadmiumgis/data/DA Ottawa.gpkg")  # GeoDataFrame with the territories poligons


# In[17]:


df.head()


# In[18]:


df_adj.head()


# In[19]:


gdf_ontario.head()


# In[20]:


template = json.loads(open("/home/binybrion/PycharmProjects/LeGenerator/scenario.json", "r").read())


# In[21]:


df_adj.head()


# In[22]:


df_adj[(df_adj["dauid"] == 35600266) | (df_adj["Neighbor_dauid"] == 35600266)]


# In[ ]:





# In[ ]:





# In[23]:


nan_rows = df[df['DApop_2016'].isnull()]
zero_pop_rows = df[df["DApop_2016"] == 0]
invalid_dauids = list(pd.concat([nan_rows, zero_pop_rows])["DAuid"])
len(invalid_dauids), len(df)


# In[24]:


adj_full = OrderedDict()  # Dictionary with the structure of the json output format

for ind, row in df_adj.iterrows():  # Iterate the different pair of adjacent territories
    if row["dauid"] in invalid_dauids:
        print("Invalid dauid found: ", row["dauid"])
        continue
    elif row["Neighbor_dauid"] in invalid_dauids:
        print("Invalid dauid found: ", row["Neighbor_dauid"])
        continue
    elif str(row["dauid"]) not in adj_full:
        rel_row = df[df["DAuid"] == row["dauid"]].iloc[0, :]
        pop = rel_row["DApop_2016"]
        area = rel_row["DAarea"]

        boundary_len = get_boundary_length(gdf_ontario, row["dauid"])
        state = {
                "age_group_proportions": [0.216, 0.279, 0.268, 0.193, 0.044],
                "susceptible": [0.5, 0.6, 0.4, 0.3, 0.2],
                "fatalities": [0, 0, 0, 0, 0],
                "infected": [
                    [0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0.8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
                ],
                "recovered": [
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                    [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                ],
                "disobedient": 0.25,
                "hospital_capacity": 0.2,
                "fatality_modifier": 1.5,
                "population": pop
            }
        adj_full[str(row["dauid"])] = {"cell_id": str(row["dauid"]), "state": state, "neighborhood": []}

    l1, l2, shared = shared_boundaries(gdf_ontario, row["dauid"], row["Neighbor_dauid"])
    correlation = (shared/l1 + shared/l2) / 2  # equation extracted from zhong paper (boundaries only, we don't have roads info for now)
    # correlation = math.e ** (-1/correlation)
    if correlation == 0:
        continue
    adj_full[str(row["dauid"])]["neighborhood"].append({"cell_id": str(row["Neighbor_dauid"]), "vicinity": {"correlation": correlation,
                                                                                                                 "infection_correction_factors": {"0.1": [0.8, 0.1], "0.2": [0.5, 0.1], "0.3": [0.2, 0.1]}}})
    if ind % 1000 == 0:
        print(ind, "%.2f%%" % (100*ind/len(df_adj)))

for key, value in adj_full.items():
    adj_full[key]["neighborhood"].append({"cell_id": key})

template["cells"] = list(adj_full.values())


# In[25]:


adj_full_json = json.dumps(template, indent=4, sort_keys=False)  # Dictionary to string (with indentation=4 for better formatting)


# In[26]:


with open("sample_output_SIIRS.json", "w") as f:
    f.write(adj_full_json)


# In[ ]:




