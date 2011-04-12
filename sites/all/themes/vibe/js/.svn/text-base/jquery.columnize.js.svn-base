/*
  Columnize Plugin for jQuery
  Version: v0.10

  Copyright (C) 2008-2010 by Lutz Issler

  Systemantics GmbH
  Am Lavenstein 3
  52064 Aachen
  GERMANY

  Web:    www.systemantics.net
  Email:  hello@systemantics.net

  This plugin is distributed under the terms of the
  GNU Lesser General Public license. The license can be obtained
  from http://www.gnu.org/licenses/lgpl.html.

*/

(function() {
	var cloneEls = new Object();
	var numColsById = new Object();
	var uniqueId = 0;

	function _layoutElement(elDOM, settings, balance) {
		// Some semi-global variables
		var colHeight;
		var colWidth;
		var col;
		var currentColEl;
		var cols = new Array();
		var colNum = 0;
		var colSet = 0;

		var el = jQuery(elDOM);

		// Save numCols property for this element
		// (needed for pagination)
		numColsById[elDOM.id] = settings.columns;

		// Remove child nodes
		el.empty();

		// Macro function (with side effects)
		function _newColumn() {
			colNum++;

			// Add a new column
			col = document.createElement("DIV");
			col.className = settings.column;
			el.append(col);
			currentColEl = col;
			colWidth = jQuery(col).width();
			cols.push(col);

			// Add the same subnode nesting to the new column
			// as there was in the old column
			for (var j=0; j<subnodes.length; j++) {
				newEl = subnodes[j].cloneNode(false);
				if (j==0 || innerContinued) {
					jQuery(newEl).addClass(settings.continued);
				}
				currentColEl.appendChild(newEl);
				currentColEl = newEl;
			}
		}

		// Returns the margin-bottom CSS property of a certain node
		function _getMarginBottom(currentColEl) {
			var marginBottom = parseInt(jQuery(currentColEl).css("marginBottom"));
			if (marginBottom.toString()=='NaN'){
				marginBottom = 0;
			}
			var currentColElParents = jQuery(currentColEl).parents();
			for (var j=0; j<currentColElParents.length; j++) {
				if (currentColElParents[j]==elDOM) {
					break;
				}
				var curMarginBottom = parseInt(jQuery(currentColElParents[j]).css("marginBottom"));
				if (curMarginBottom.toString()!='NaN'){
					marginBottom = Math.max(marginBottom, curMarginBottom);
				}
			}
			return marginBottom;
		}

		// Advance to next sibling on el or a parent level
		function _skipToNextNode() {
			while (currentEl && currentColEl && !currentEl.nextSibling) {
				currentEl = currentEl.parentNode;
				currentColEl = currentColEl.parentNode;
				var node = subnodes.pop();
				// Hack: delete the previously saved HREF
				if (node=="A") {
					href = null;
				}
			}
			if (currentEl) {
				currentEl = currentEl.nextSibling;
			}
		}

		// Take the height from the element to be layouted
		var maxHeight = settings.height
			? settings.height
			: parseInt(el.css("maxHeight"));
		if (balance || isNaN(maxHeight) || maxHeight==0) {
			// We are asked to balance the col lengths
			// or cannot get the column length from the container,
			// so chose a height that will produce >numCols< columns
			col = document.createElement("DIV");
			col.className = settings.column;
			jQuery(col).append(jQuery(cloneEls[elDOM.id]).html());
			el.append(col);
			var lineHeight = parseInt(el.css("lineHeight"));
			if (!lineHeight) {
				// Assume a line height of 120%
				lineHeight = Math.ceil(parseInt(el.css("fontSize"))*1.2);
			}
			colHeight = Math.ceil(jQuery(col).height()/settings.columns);
			if (colHeight%lineHeight>0) {
				colHeight += lineHeight;
			}
			elDOM.removeChild(col);
			if (maxHeight>0 && colHeight>maxHeight) {
				// Balance only to max-height
				colHeight = maxHeight;
			}
		} else {
			colHeight = maxHeight;
		}

		// Take the minimum height into account
		var minHeight = settings.minHeight
			? settings.minHeight
			: parseInt(el.css("minHeight"));
		if (minHeight) {
			colHeight = Math.max(colHeight, minHeight);
		}

		// Start with first child of the initial node
		var currentEl = cloneEls[elDOM.id].children(":first")[0];
		var subnodes = new Array();
		var href = null;
		var lastNodeType = 0;
		_newColumn();
		if (colHeight==0 || colWidth==0) {
			// We cannot continue with zero height or width
			return false;
		}
		while (currentEl) {
			if (currentEl.nodeType==1) {
				// An element node
				var newEl;
				var $currentEl = jQuery(currentEl);
				if ($currentEl.hasClass("dontSplit")
					|| $currentEl.is(settings.dontsplit)) {
					// Don't split this node. Instead, clone it completely
					var newEl = currentEl.cloneNode(true);
					currentColEl.appendChild(newEl);
					if (col.offsetHeight>colHeight) {
						// The column gets too long, start a new colum
						_newColumn();
					}
					_skipToNextNode();
				} else {
					// Clone the node and append it to the current column
					var newEl = currentEl.cloneNode(false);
					currentColEl.appendChild(newEl);
					if (col.offsetHeight-_getMarginBottom(currentColEl)>colHeight) {
						// The column gets too long, start a new colum
						currentColEl.removeChild(newEl);
						var toBeInsertedEl = newEl;
						_newColumn();
						currentColEl.appendChild(toBeInsertedEl);
						newEl = toBeInsertedEl;
					}
					if (currentEl.firstChild) {
						subnodes.push(currentEl.cloneNode(false));
						currentColEl = newEl;
						currentEl = currentEl.firstChild;
					} else {
						_skipToNextNode();
					}
				}
				lastNodeType = 1;
			} else if (currentEl.nodeType==3) {
				// A text node
				var newEl = document.createTextNode("");
				currentColEl.appendChild(newEl);
				// Determine the current bottom margin
				var marginBottom = _getMarginBottom(currentColEl);
				// Append word by word
				var words = currentEl.data.split(" ");
				for (var i=0; i<words.length; i++) {
					if (lastNodeType==3) {
						newEl.appendData(" ");
					}
					newEl.appendData(words[i]);
					currentColEl.removeChild(newEl);
					currentColEl.appendChild(newEl);
					if (col.offsetHeight-marginBottom>colHeight) {
						// el column is full
						// Remove the last word
						newEl.data = newEl.data.substr(0, newEl.data.length-words[i].length-1);

						// Remove the last node if empty
						var innerContinued;
						if (jQuery(currentColEl).text()=="") {
							jQuery(currentColEl).remove();
							innerContinued = false;
						} else {
							innerContinued = true;
						}

						// Start a new column
						_newColumn();

						// Add a text node at the bottom level
						// in order to continue the column
						newEl = document.createTextNode(words[i]);
						currentColEl.appendChild(newEl);
					}
					lastNodeType = 3;
				}
				_skipToNextNode();
				lastNodeType = 0;
			} else {
				// Any other node (comments, for instance)
				_skipToNextNode();
				lastNodeType = currentEl.nodeType;
			}
		}
		return cols;
	};

	jQuery.fn.columnize = function(settings) {
		settings = jQuery.extend({
			column: "column",
			continued: "continued",
			columns: 2,
			balance: true,
			height: false,
			minHeight: false,
			cache: true,
			dontsplit: ""
		}, settings);
		this.each(function () {
			var jthis = jQuery(this);

			var id = this.id;
			if (!id) {
				// Get a new id
				id = "jcols_"+uniqueId;
				this.id = id;
				uniqueId++;
			}

			if (!cloneEls[this.id] || !settings.cache) {
				cloneEls[this.id] = jthis.clone(true);
			}

			// Layout the columns
			var cols = _layoutElement(this, settings, settings.balance);
			if (!cols) {
				// Layout failed, restore the object's contents
				jthis.append(cloneEls[this.id].children().clone(true));
			}
		});
		return this;
	}
})();
