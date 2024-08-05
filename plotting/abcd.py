#!/usr/bin/env python

# %%
import matplotlib.pyplot as plt
import mplhep as hep
plt.rcParams.update({"figure.facecolor":  (1,1,1,0),
                     "figure.figsize":  [10, 8],})

# %%
cuts = ["abcdnet_score > 0.92", "BDT Score > 0.56", "hbbfatjet_xbb > 0.5", "vqqfatjet_xqq > 0.7"]
x_arm = cuts[0]
y_arm = cuts[1]
presel = cuts[2:]


x_mid = "abcdnet_score > 0.4"
y_mid = "vbs_bdt_score > 0.4"


A = " and ".join([x_arm, y_arm, *presel])
B = " and ".join([x_arm, f"(not {y_arm})", *presel])
C = " and ".join([f"(not {x_arm})", y_arm, *presel])
D = " and ".join([f"(not {x_arm})", f"(not {y_arm})", *presel])

B1 = " and ".join([x_arm, f"({y_mid}) and (not {y_arm})", *presel])
B2 = " and ".join([x_arm, f"(not {y_mid}) and (not {y_arm})", *presel])
C1 = " and ".join([f"({x_mid}) and (not {x_arm})", y_arm, *presel])
C2 = " and ".join([f"(not {x_mid}) and (not {x_arm})", y_arm, *presel])
D1 = " and ".join([f"({x_mid}) and (not {x_arm})", f"({y_mid}) and (not {y_arm})", *presel])
D2 = " and ".join([f"({x_mid}) and (not {x_arm})", f"(not {y_mid}) and (not {y_arm})", *presel])
D3 = " and ".join([f"(not {x_mid}) and (not {x_arm})", f"({y_mid}) and (not {y_arm})", *presel])
D4 = " and ".join([f"(not {x_mid}) and (not {x_arm})", f"(not {y_mid}) and (not {y_arm})", *presel])

# %%
TAG = "abcdnet_bdt"

# %%
import matplotlib.patches as patches
        
def plot_abcd(x_arm, y_arm, x_mid=None, y_mid=None, presel_cuts=None, cr=False):
    
    plot_x_mid = (not x_mid is None)
    plot_xy_mid = (plot_x_mid and not y_mid is None)
    
    fig, axes = plt.subplots()

    if TAG == "abcdnet_bdt" or TAG == "abcdnet_twin" or TAG == "abcdnet_double":
        ylim = [0, 1]
    elif TAG == "abcdnet_detajj":
        ylim = [0, 10]

    axes.set_ylim(ylim)

    tx_size = 32
    # Move the left and bottom spines to x = 0 and y = 0, respectively.
    axes.spines[["left", "bottom"]].set_position(("data", 0))
    # Hide the top and right spines.
    axes.spines[["top", "right"]].set_visible(False)
    axes.spines[["left", "bottom"]].set_linewidth(4)

    axes.plot(1, 0, ">k", markersize=15, clip_on=False)
    axes.plot(*ylim, "^k", markersize=15, clip_on=False)

    axes.set_xlabel("ABCDNet score", size=tx_size)

    x_wp = float(x_arm.split(" > ")[-1])
    y_wp = float(y_arm.split(" > ")[-1])
    if plot_x_mid:
        x_md = float(x_mid.split(" > ")[-1])
    if plot_xy_mid:
        y_md = float(y_mid.split(" > ")[-1])
    
    axes.axhline(y_wp, color="k", linewidth=4)
    axes.axvline(x_wp, color="k", linewidth=4)
    if plot_x_mid:
        axes.axvline(x_md, color="grey", linewidth=4)
    if plot_xy_mid:
        axes.axhline(y_md, color="grey", linewidth=4)

    if TAG == "abcdnet_bdt":
        axes.set_ylabel("BDT score", size=tx_size)
    elif TAG == "abcdnet_detajj":
        axes.set_ylabel("$|\Delta\eta_{jj}|$", size=tx_size)
    elif TAG == "abcdnet_twin" or TAG == "abcdnet_double":
        axes.set_ylabel("Other ABCDNet score", size=tx_size)

    text_kwargs = {"horizontalalignment": "center", "verticalalignment": "center", "size": tx_size}

    if not cr:
        if plot_xy_mid:
            axes.text((1 + x_wp)/2, (ylim[-1] + y_wp)/2, "A", **text_kwargs)
            axes.text((x_wp + x_md)/2, (ylim[-1] + y_wp)/2, "C$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (ylim[-1] + y_wp)/2, "C$_2$", **text_kwargs)

            axes.text((1 + x_wp)/2, (y_wp + y_md)/2, "B$_1$", **text_kwargs)
            axes.text((x_wp + x_md)/2, (y_wp + y_md)/2, "D$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (y_wp + y_md)/2, "D$_3$", **text_kwargs)

            axes.text((1 + x_wp)/2, (y_md + 0)/2, "B$_2$", **text_kwargs)
            axes.text((x_wp + x_md)/2, (y_md + 0)/2, "D$_2$", **text_kwargs)
            axes.text((x_md + 0)/2, (y_md + 0)/2, "D$_4$", **text_kwargs)
        elif plot_x_mid:
            axes.text((1 + x_wp)/2, (ylim[-1] + y_wp)/2, "A", **text_kwargs)
            axes.text((x_wp + x_md)/2, (ylim[-1] + y_wp)/2, "C$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (ylim[-1] + y_wp)/2, "C$_2$", **text_kwargs)

            axes.text((1 + x_wp)/2, (y_wp + 0)/2, "B", **text_kwargs)
            axes.text((x_wp + x_md)/2, (y_wp + 0)/2, "D$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (y_wp + 0)/2, "D$_2$", **text_kwargs)
        else:
            axes.text((1 + x_wp)/2, (ylim[-1] + y_wp)/2, "A", **text_kwargs)
            axes.text((1 + x_wp)/2, (y_wp + 0)/2, "B", **text_kwargs)
            axes.text((0 + x_wp)/2, (ylim[-1] + y_wp)/2, "C", **text_kwargs)
            axes.text((x_wp + 0)/2, (y_wp + 0)/2, "D", **text_kwargs)
    else:
        if plot_xy_mid:
            axes.text((1 + x_wp)/2, (ylim[-1] + y_wp)/2, "A'", **text_kwargs)
            axes.text((x_wp + x_md)/2, (ylim[-1] + y_wp)/2, "C$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (ylim[-1] + y_wp)/2, "C$_2$", **text_kwargs)

            axes.text((1 + x_wp)/2, (y_wp + y_md)/2, "B$_1$", **text_kwargs)
            axes.text((x_wp + x_md)/2, (y_wp + y_md)/2, "D$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (y_wp + y_md)/2, "D$_3$", **text_kwargs)

            axes.text((1 + x_wp)/2, (y_md + 0)/2, "B$_2$", **text_kwargs)
            axes.text((x_wp + x_md)/2, (y_md + 0)/2, "D$_2$", **text_kwargs)
            axes.text((x_md + 0)/2, (y_md + 0)/2, "D$_4$", **text_kwargs)
        elif plot_x_mid:
            axes.text((1 + x_wp)/2, (ylim[-1] + y_wp)/2, "A", **text_kwargs)
            axes.text((x_wp + x_md)/2, (ylim[-1] + y_wp)/2, "C$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (ylim[-1] + y_wp)/2, "C$_2$", **text_kwargs)

            axes.text((1 + x_wp)/2, (y_wp + 0)/2, "B'", **text_kwargs)
            axes.text((x_wp + x_md)/2, (y_wp + 0)/2, "D$_1$", **text_kwargs)
            axes.text((x_md + 0)/2, (y_wp + 0)/2, "D$_2$", **text_kwargs)
        else:
            axes.text((1 + x_wp)/2, (ylim[-1] + y_wp)/2, "A'", **text_kwargs)
            axes.text((1 + x_wp)/2, (y_wp + 0)/2, "B'", **text_kwargs)
            axes.text((0 + x_wp)/2, (ylim[-1] + y_wp)/2, "C'", **text_kwargs)
            axes.text((x_wp + 0)/2, (y_wp + 0)/2, "D'", **text_kwargs)

    if plot_xy_mid:
        axes.set_xticks([x_md, x_wp])
        axes.set_yticks([y_md, y_wp])
    elif plot_x_mid:
        axes.set_xticks([x_md, x_wp])
        axes.set_yticks([y_wp])
    else:
        axes.set_xticks([x_wp])
        axes.set_yticks([y_wp])
    
    # Remove all margins
    axes.margins(0)
    # Remove CMS-style ticks on top and right sides of plot
    axes.tick_params(top=False, right=False, labelsize=24, width=4)
    axes.tick_params(top=False, bottom=False, right=False, left=False, which="minor")
    
    # Create a Rectangle patch for each region
    A_rect = patches.Rectangle(
        (x_wp, y_wp), (1 - x_wp), (ylim[-1] - y_wp), 
        linewidth=0, facecolor="#61D836", alpha=0.25
    )
    B_rect = patches.Rectangle(
        (x_wp, 0), (1 - x_wp), (y_wp), 
        linewidth=0, facecolor="#56C1FF", alpha=0.25
    )
    C_rect = patches.Rectangle(
        (0, y_wp), (x_wp), (ylim[-1] - y_wp), 
        linewidth=0, facecolor="#B51700", alpha=0.25
    )
    D_rect = patches.Rectangle(
        (0, 0), (x_wp), (y_wp), 
        linewidth=0, facecolor="#FFD932", alpha=0.25
    )

    # Add the patches to the Axes
    axes.add_patch(A_rect)
    axes.add_patch(B_rect)
    axes.add_patch(C_rect)
    axes.add_patch(D_rect)

    if not presel_cuts is None:
        title = " and ".join(presel_cuts)
        # Hard-coded replacements
        title = title.replace("vqqfatjet_xqq", r"XWqq($V\rightarrow qq$)")
        title = title.replace("hbbfatjet_xbb", r"Xbb($H\rightarrow bb$)")
        # Hard-coded length limit (make newline)
        if len(title) > 100:
            title = title.split(" and ")
            title[-1] = "\n" + title[-1]
            title = " and ".join(title)
        
        axes.set_title(title, size=24, pad=24)
    
    axes.patch.set_alpha(0)
    if not cr:
        plt.savefig(f"plots/ABCDscheme.png", bbox_inches="tight")
    else:
        plt.savefig(f"plots/ABCDscheme2.png", bbox_inches="tight")

plot_abcd(x_arm, y_arm, presel_cuts=presel)
plot_abcd(x_arm, y_arm, presel_cuts=["hbbfatjet_xbb < 0.95", "vqqfatjet_xqq < 0.7"], cr=True)