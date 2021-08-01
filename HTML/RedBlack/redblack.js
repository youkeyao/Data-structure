function insert() {
  let data = getInput();
  if (!data) return;
  input.value = "";
  let element = createElement(data, root);
  element.children[0].style.color = "white";
  element.children[0].style.backgroundColor = "red";

  let n = new Tree_Node(data, null, null, null, 0, 0, element);
  disableInput();
  new Promise((resolve, reject) => {
    let choosing = createElement(null, root);
    choosing.children[0].classList.toggle("choosing");
    choosing.style.top = "60px";
    choosing.style.left = (60+preorder_list.indexOf(tree_root)*80)+"px";
    search(choosing, tree_root, data).then((result) => {
      root.removeChild(choosing);
      if (insert_node(result, n)) {
        resolve();
      }
      else {
        enableInput();
        reject("Insert failed");
      }
    });
  }).then(() => {
    update(preorder_list.indexOf(n)).then(() => {
      check_RB_insert(n);
    });;
  });
}

function remove() {
  let data = getInput();
  let old_color;
  if (!data) return;
  input.value = "";

  disableInput();

  new Promise((resolve, reject) => {
    let choosing = createElement(null, root);
    choosing.children[0].classList.toggle("choosing");
    choosing.style.top = "60px";
    choosing.style.left = (60+preorder_list.indexOf(tree_root)*80)+"px";
    search(choosing, tree_root, data).then((result) => {
      root.removeChild(choosing);
      if (result && result.data == data) {
        let pos = preorder_list.indexOf(result);
        let res = remove_node(result);
        old_color = preorder_list[pos].element.children[0].style.backgroundColor;
        if (!res) {
          preorder_list[pos].element.children[0].style.backgroundColor = preorder_list[pos+1].element.children[0].style.backgroundColor;
          preorder_list[pos+1].element.children[0].style.backgroundColor = old_color;
          Velocity(preorder_list[pos].element, {
            top: preorder_list[pos+1].element.offsetTop + "px",
            left: preorder_list[pos+1].element.offsetLeft + "px"
          }, {
            duration: DURATION
          });
          Velocity(preorder_list[pos+1].element, {
            top: preorder_list[pos].element.offsetTop + "px",
            left: preorder_list[pos].element.offsetLeft + "px"
          }, {
            duration: DURATION
          }).then(() => {
            let parent = preorder_list[pos+1].parent;
            let node = preorder_list[pos+1].right;
            let element = preorder_list[pos+1].element;
            preorder_list.splice(pos+1, 1);
            Velocity(element, {
              top: "-50px",
              left: "-50px"
            }, {
              duration: DURATION
            }).then(() => {
              root.removeChild(element);
            });
            resolve([parent, node]);
          });
        }
        else {
          let parent = preorder_list[pos].parent;
          let node = preorder_list[pos].left ? preorder_list[pos].left : preorder_list[pos].right;
          let element = preorder_list[pos].element;
          preorder_list.splice(pos, 1);
            Velocity(element, {
              top: "-50px",
              left: "-50px"
            }, {
              duration: DURATION
            }).then(() => {
              root.removeChild(element);
            });
          resolve([parent, node]);
        }
      }
      else {
        enableInput();
        reject("Search failed");
      }
    });
  }).then((result) => {
    let pos = preorder_list.indexOf(result[1]);
    update(pos >= 0 ? pos : 0).then(() => {
      check_RB_remove(result[0], result[1], old_color);
    });
  });
}

async function check_RB_insert(node) {
  if (!node.parent) {
    node.element.children[0].style.backgroundColor = "black";
  }
  else if (node.parent.element.children[0].style.backgroundColor == "red") {
    let uncle = getUncle(node);
    if (uncle && uncle.element.children[0].style.backgroundColor == "red") {
      node.parent.element.children[0].style.backgroundColor = "black";
      uncle.element.children[0].style.backgroundColor = "black";
      node.parent.parent.element.children[0].style.backgroundColor = "red";
      await update(0);
      check_RB_insert(node.parent.parent);
      return;
    }
    else {
      let g = node.parent.parent;
      let f = node.parent;
      if (g.left == f) {
        if (f.left == node) {
          connect34(node, f, g, node.left, node.right, f.right, g.right, g, 1);
          g.element.children[0].style.backgroundColor = "red";
          f.element.children[0].style.backgroundColor = "black";
        }
        else {
          connect34(f, node, g, f.left, node.left, node.right, g.right, g, 2);
          g.element.children[0].style.backgroundColor = "red";
          node.element.children[0].style.backgroundColor = "black";
        }
      }
      else {
        if (f.left == node) {
          connect34(g, node, f, g.left, node.left, node.right, f.right, g, 2);
          g.element.children[0].style.backgroundColor = "red";
          node.element.children[0].style.backgroundColor = "black";
        }
        else {
          connect34(g, f, node, g.left, f.left, node.left, node.right, g, 1);
          g.element.children[0].style.backgroundColor = "red";
          f.element.children[0].style.backgroundColor = "black";
        }
      }
    }
  }
  update(0);
}

async function check_RB_remove(parent, node, old_color) {
  if (!parent) {
    if (node) node.element.children[0].style.backgroundColor = "black";
  }
  else if (old_color == "black") {
    let brother = getBrother(parent, node);
    if (brother.element.children[0].style.backgroundColor == "red") {
      if (parent.left == node) {
        brother.element.children[0].style.backgroundColor = "black";
        parent.element.children[0].style.backgroundColor = "red";
        connect34(parent, brother, brother.right, node, brother.left, brother.right.left, brother.right.right, parent, 1);
      }
      else {
        brother.element.children[0].style.backgroundColor = "black";
        parent.element.children[0].style.backgroundColor = "red";
        connect34(brother.left, brother, parent, brother.left.left, brother.left.right, brother.right, node, parent, 1);
      }
      await update(0);
      check_RB_remove(parent, node, "black");
      return;
    }
    else {
      if (brother.left && brother.left.element.children[0].style.backgroundColor == "red") {
        if (parent.right == node) {
          brother.element.children[0].style.backgroundColor = parent.element.children[0].style.backgroundColor;
          parent.element.children[0].style.backgroundColor = "black";
          brother.left.element.children[0].style.backgroundColor = "black";
          connect34(brother.left, brother, parent, brother.left.left, brother.left.right, brother.right, node, parent, 1);
        }
        else {
          brother.left.element.children[0].style.backgroundColor = parent.element.children[0].style.backgroundColor;
          parent.element.children[0].style.backgroundColor = "black";
          connect34(parent, brother.left, brother, node, brother.left.left, brother.left.right, brother.right, parent, 2);
        }
      }
      else if (brother.right && brother.right.element.children[0].style.backgroundColor == "red") {
        if (parent.left == node) {
          brother.element.children[0].style.backgroundColor = parent.element.children[0].style.backgroundColor;
          parent.element.children[0].style.backgroundColor = "black";
          brother.right.element.children[0].style.backgroundColor = "black";
          connect34(parent, brother, brother.right, node, brother.left, brother.right.left, brother.right.right, parent, 1);
        }
        else {
          brother.right.element.children[0].style.backgroundColor = parent.element.children[0].style.backgroundColor;
          parent.element.children[0].style.backgroundColor = "black";
          connect34(brother, brother.right, parent, brother.left, brother.right.left, brother.right.right, node, parent, 2);
        }
      }
      else {
        brother.element.children[0].style.backgroundColor = "red";
        if (parent.element.children[0].style.backgroundColor == "red") {
          parent.element.children[0].style.backgroundColor = "black";
        }
        else {
          check_RB_remove(parent.parent, parent, "black");
        }
      }
    }
  }
  update(0);
}

function getUncle(node) {
  let p = node.parent;
  if (p.parent.left == p) {
    return p.parent.right;
  }
  else {
    return p.parent.left;
  }
}

function getBrother(parent, node) {
  if (parent.left == node) {
    return parent.right;
  }
  else {
    return parent.left;
  }
}