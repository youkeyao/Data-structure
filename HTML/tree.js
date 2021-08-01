let tree_root = null;
const preorder_list = [];

function Tree_Node(data, parent, left, right, height, depth, element) {
  this.data = data;
  this.parent = parent;
  this.left = left;
  this.right = right;
  this.height = height;
  this.depth = depth;
  this.element = element;
}

function update_depth(node) {
  let queue = [node];
  while(queue.length != 0) {
    let e = queue.shift();
    if (e.left) {
      e.left.depth = e.depth + 1;
      queue.push(e.left);
    }
    if (e.right) {
      e.right.depth = e.depth + 1;
      queue.push(e.right);
    }
  }
}

function update_height(node) {
  if (!node) return;
  let left_height = node.left ? node.left.height : -1;
  let right_height = node.right ? node.right.height : -1;
  node.height = Math.max(left_height, right_height) + 1;
}

function update_height_above(node) {
  while(node) {
    update_height(node);
    node = node.parent;
  }
}

function change_parent_pointer(node, tochange) {
  let parent = node.parent;
  if (!parent) {
    tree_root = tochange;
  }
  else {
    if (parent.left == node) {
      parent.left = tochange;
    }
    else {
      parent.right = tochange;
    }
  }
}

function connect34(l_node, m_node, r_node, T0, T1, T2, T3, g, d) {
  let p = g.parent;
  change_parent_pointer(g, m_node);
  if (T0) {
    T0.parent = l_node;
  }
  if (T1) {
    T1.parent = l_node;
  }
  if (T2) {
    T2.parent = r_node;
  }
  if (T3) {
    T3.parent = r_node;
  }
  if (l_node) {
    l_node.left = T0;
    l_node.right = T1;
    l_node.parent = m_node;
  }
  if (r_node) {
    r_node.left = T2;
    r_node.right = T3;
    r_node.parent = m_node;
  }
  m_node.left = l_node;
  m_node.right = r_node;
  m_node.parent = p;
  update_height(l_node);
  update_height(r_node);
  update_height(m_node);
  m_node.depth -= d;
  update_depth(m_node);
  return m_node;
}

function search(choosing, node, data) {
  return new Promise((resolve) => {
    if (!node) {
      resolve(null);
    }
    else {
      Velocity(choosing, {
        top: node.element.offsetTop+"px",
        left: node.element.offsetLeft+"px"
      }, {
        duration: DURATION
      }).then(() => {
        if ((data < node.data && !node.left) || (data > node.data && !node.right) || (data == node.data)) {
          setTimeout(() => {
            resolve(node);
          }, DURATION);
        }
        else {
          search(choosing, data < node.data ? node.left : node.right, data).then((result) => {
            resolve(result);
          });
        }
      });
    }
  });
}

function insert_node(parent, node) {
  if (!parent) {
    tree_root = node;
    preorder_list.splice(0, 0, node);
  }
  else if (node.data == parent.data) {
    return false;
  }
  else if (node.data < parent.data) {
    parent.left = node;
    node.depth = parent.depth + 1;
    node.parent = parent;
    preorder_list.splice(preorder_list.indexOf(parent), 0, node);
    update_height_above(parent);
  }
  else {
    parent.right = node;
    node.depth = parent.depth + 1;
    node.parent = parent;
    preorder_list.splice(preorder_list.indexOf(parent) + 1, 0, node);
    update_height_above(parent);
  }
  return true;
}

function remove_node(node) {
  let pos = preorder_list.indexOf(node);
  if (!node.left) {
    if (node.right) {
      node.right.parent = node.parent;
      node.right.depth --;
      update_depth(node.right);
    }
    change_parent_pointer(node, node.right);
    update_height_above(node.parent);
    return true;
  }
  else if (!node.right) {
    if (node.left) {
      node.left.parent = node.parent;
      node.left.depth --;
      update_depth(node.left);
    }
    change_parent_pointer(node, node.left);
    update_height_above(node.parent);
    return true;
  }
  else {
    let tmp = preorder_list[pos].element;
    preorder_list[pos].data = preorder_list[pos+1].data;
    preorder_list[pos].element = preorder_list[pos+1].element;
    preorder_list[pos+1].element = tmp;
    remove_node(preorder_list[pos+1]);
    return false;
  }
}

function update(pos) {
  return new Promise((resolve) => {
    for (let i = pos; i < preorder_list.length; i++) {
      Velocity(preorder_list[i].element, {
        top: (60+preorder_list[i].depth*80)+"px",
        left: (60+i*80)+"px"
      }, {
        duration: DURATION
      });
    }
    let id = setInterval(() => {
      canvas.width = root.scrollWidth;
      canvas.height = root.scrollHeight;
      if (tree_root) {
        context.moveTo(tree_root.element.offsetLeft, 0);
        context.lineTo(tree_root.element.offsetLeft, tree_root.element.offsetTop);
        context.stroke();
        type_select.disabled = true;
      }
      else {
        type_select.disabled = false;
      }
      let queue = [tree_root];
      while (queue.length != 0) {
        let e = queue.shift();
        if (e && e.left) {
          queue.push(e.left);
          context.moveTo(e.element.offsetLeft, e.element.offsetTop);
          context.lineTo(e.left.element.offsetLeft, e.element.offsetTop);
          context.stroke();
          context.moveTo(e.left.element.offsetLeft, e.element.offsetTop);
          context.lineTo(e.left.element.offsetLeft, e.left.element.offsetTop);
          context.stroke();
        }
        if (e && e.right) {
          queue.push(e.right);
          context.moveTo(e.element.offsetLeft, e.element.offsetTop);
          context.lineTo(e.right.element.offsetLeft, e.element.offsetTop);
          context.stroke();
          context.moveTo(e.right.element.offsetLeft, e.element.offsetTop);
          context.lineTo(e.right.element.offsetLeft, e.right.element.offsetTop);
          context.stroke();
        }
      }
    }, 10);
    setTimeout(() => {
      clearInterval(id);
      enableInput();
      resolve();
    }, DURATION + INTERVAL);
  });
}