function vocab(id) {

  if (id == 0) {
    divTermSelect = document.getElementById("div_termselect");

    while (divTermSelect.hasChildNodes()) {
      divTermSelect.removeChild(divTermSelect.firstChild);
    }
  }
  else {
    termSelect = document.getElementById("term_select_"+id).cloneNode(true);
    divTermSelect = document.getElementById("div_termselect");
    termSelect.style.display = "block";

    while (divTermSelect.hasChildNodes()) {
      divTermSelect.removeChild(divTermSelect.firstChild);
    }

    divTermSelect.appendChild(termSelect);
    legendDiv = divTermSelect.appendChild(document.createElement("div"));
    legendDiv.innerHTML = "If you do not select a term, this rule will apply to all terms in this vocabulary.";
    divTermSelect.appendChild(legendDiv);
  }
}

function term(id) {
  document.getElementById("term_id").value=id;
}