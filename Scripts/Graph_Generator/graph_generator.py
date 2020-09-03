# Original author: Kevin
# Modified by: Binyamin

#!/usr/bin/env python
# coding: utf-8

# In[1]:


import os
import re
from collections import defaultdict
from copy import copy


# In[2]:

log_file_folder = "../../logs"
log_filename = log_file_folder + "/pandemic_outputs.txt"
#dim = 10,10
#population_per_cell = 100
#population = dim[0] * dim[1] * population_per_cell
patt_out_line = "\{(?P<id>.*) ; <(?P<state>[\w,. -]+)>\}"


# In[3]:


COLOR_SUSCEPTIBLE = "#3498db"
COLOR_INCUBATION = "#f1c40f"
COLOR_INFECTED = "#e74c3c"
COLOR_LATENT = "#c0392b"
COLOR_RECOVERED = "#2ecc71"
COLOR_DEAD = "#9b59b6"


# In[4]:


def curr_states_to_df_row(sim_time, curr_states, num_inf, num_rec, line_num):
    sus_acc = 0
    inf_acc = 0
    rec_acc = 0
    dth_acc = 0
    
    new_inf = 0
    new_rec = 0

    for k in curr_states:
        sus_acc += curr_states[k][1]
        inf_acc += curr_states[k][2]
        rec_acc += curr_states[k][3]
        #dth_acc += curr_states[k][num_inf+num_rec+1]

        new_inf += curr_states[k][4]
        new_rec += curr_states[k][5]
        current_deaths = curr_states[k][6]
        dth_acc += current_deaths

        #print(sum(curr_states[k]), sus_acc, inf_acc, rec_acc, dth_acc)
        assert 0.95 <= sum(curr_states[k][1:4]) + current_deaths < 1.05, (curr_time, sum(curr_states[k][1:4]), dth_acc, line_num)
            
    num_cells = len(curr_states)
    sus_acc /= num_cells
    inf_acc /= num_cells
    rec_acc /= num_cells
    dth_acc /= num_cells


    new_inf /= num_cells
    new_rec /= num_cells
    
    assert 0.95 <= sus_acc + inf_acc + rec_acc + dth_acc < 1.05, (curr_time, sus_acc, inf_acc, rec_acc, dth_acc, num_cells)
    
    return [int(sim_time), sus_acc, inf_acc, rec_acc, new_inf, new_rec, dth_acc]


# In[5]:


states = ["sus", "infec", "rec"]
data = []
curr_data = []
curr_time = None
curr_states = {}
num_inf = 0
num_rec = 0

with open(log_filename, "r") as log_file:
    line_num = 0
    for line in log_file:
        line = line.strip()
        if line.isnumeric() and line != curr_time:
            if curr_states:
                data.append(curr_states_to_df_row(curr_time, curr_states, num_inf, num_rec, line_num))
            curr_time = line
            continue

        match = re.search(patt_out_line, line)
        if not match:
            print(line)
            continue
            
        if not curr_states:
            sp = match.group("state").split(",")

        cid = match.group("id")
        state = list(map(float, match.group("state").split(",")))
        curr_states[cid] = state
        line_num += 1
        
    data.append(curr_states_to_df_row(curr_time, curr_states, num_inf, num_rec, line_num))


# In[ ]:


data[:3]


# ### Visualization

# In[ ]:


import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib


# In[ ]:


font = {'family' : 'Times New Roman',
        'weight' : 'normal',
        'size'   : 16}

matplotlib.rc('font', **font)


# In[ ]:


columns = ["time", "susceptible", "infected", "recovered", "new_infected", "new_recovered", "deaths"]
df_vis = pd.DataFrame(data, columns=columns)
df_vis = df_vis.set_index("time")
df_vis.to_csv("states.csv")
df_vis.head()


# In[ ]:


base_name = log_file_folder + "/pandemic_outputs"

col_names = ["infected", "recovered"]
colors=[COLOR_INFECTED, COLOR_RECOVERED]

x = list(df_vis.index)
y = np.vstack([df_vis[col] for col in col_names])

fig, ax = plt.subplots(figsize=(12,7))
ax.stackplot(x, y, labels=col_names, colors=colors)
plt.legend(loc='upper right')
plt.margins(0,0)
#plt.title('Epidemic percentages (%s)' % base_name)
#plt.show()
plt.xlabel("Time (days)")
plt.ylabel("Population (%)")
plt.savefig(base_name + "_area.pdf")
plt.savefig(base_name + "_area.png")


# In[ ]:


fig, ax = plt.subplots(figsize=(15,6))
linewidth = 2

x = list(df_vis.index)
ax.plot(x, df_vis["susceptible"], label="Susceptible", color=COLOR_SUSCEPTIBLE, linewidth=linewidth)
ax.plot(x, df_vis["infected"], label="Infected", color=COLOR_INFECTED, linewidth=linewidth, linestyle="--")
ax.plot(x, df_vis["recovered"], label="Recovered", color=COLOR_RECOVERED, linewidth=linewidth, linestyle="-.")
#ax.plot(x, df_vis["deaths"], label="Deaths", color=COLOR_DEAD, linewidth=linewidth, linestyle=":")
plt.legend(loc='upper right')
plt.margins(0,0)
#plt.title('Epidemic percentages (%s)' % base_name)
plt.xlabel("Time (days)")
plt.ylabel("Population (%)")
plt.savefig(base_name + "_lines.pdf")
plt.savefig(base_name + "_lines.png")


# In[ ]:


fig, ax = plt.subplots(figsize=(15,6))
linewidth = 2

x = list(df_vis.index)
ax.plot(x, df_vis["new_infected"], label="New infected", color=COLOR_INFECTED, linewidth=linewidth, linestyle="--")
ax.plot(x, df_vis["new_recovered"], label="New recovered", color=COLOR_RECOVERED, linewidth=linewidth, linestyle="-.")
plt.legend(loc='upper right')
plt.margins(0,0)
#plt.title('Epidemic percentages (%s)' % base_name)
plt.xlabel("Time (days)")
plt.ylabel("Population (%)")
#plt.savefig(base_name + "_lines.pdf")


# In[ ]:


fig, axs = plt.subplots(3, figsize=(15,6))
linewidth = 2

x = list(df_vis.index)
axs[0].plot(x, df_vis["susceptible"], label="Susceptible", color=COLOR_SUSCEPTIBLE, linewidth=linewidth)
axs[0].plot(x, df_vis["infected"], label="Infected", color=COLOR_INFECTED, linewidth=linewidth, linestyle="--")
axs[0].plot(x, df_vis["recovered"], label="Recovered", color=COLOR_RECOVERED, linewidth=linewidth, linestyle="-.")
axs[0].set_ylabel("Population (%)")
axs[0].legend()

axs[1].plot(x, df_vis["new_infected"], label="New infected", color=COLOR_INFECTED, linewidth=linewidth, linestyle="--")
axs[1].plot(x, df_vis["new_recovered"], label="New recovered", color=COLOR_RECOVERED, linewidth=linewidth, linestyle="-.")
axs[1].set_ylabel("Population (%)")
axs[1].legend()

axs[2].plot(x, df_vis["deaths"], label="Deaths", color=COLOR_DEAD, linewidth=linewidth, linestyle=":")
axs[2].set_xlabel("Time (days)")
axs[2].set_ylabel("Population (%)")
axs[2].legend()

#plt.margins(0,0)
#plt.title('Epidemic percentages (%s)' % base_name)
plt.savefig(base_name + "_lines_with_news.pdf")
plt.savefig(base_name + "_lines_with_news.png")

#handles, labels = axs[0].get_legend_handles_labels()
#fig.legend(handles, labels, loc=(0.808,0.295))


# In[ ]:




