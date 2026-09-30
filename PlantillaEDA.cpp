#include <iostream>
#include <vector>
using namespace::std;






//------------------------------------------------
// -------PERSISTENT SEGMENT TREE--------------------------
//------------------------------------------------

template<typename data_type>
struct PersistentSegmentTree {

    struct SegmentTreeNode {
        data_type data;
        int l, r;
        SegmentTreeNode *left, *right;
    };

    vector<SegmentTreeNode*> version_roots;

    PersistentSegmentTree(int n) {
        version_roots.emplace_back(new SegmentTreeNode(data_type(), 0, n - 1, nullptr, nullptr));
        build(version_roots[0]);
    }

    PersistentSegmentTree(int l, int r, vector<data_type> &a) {
        version_roots.emplace_back(new SegmentTreeNode(data_type(), l, r, nullptr, nullptr));
        build(version_roots[0], a);
    }

    void build(SegmentTreeNode *root) {
        if (root -> l == root -> r) {
            // -1 porque indexamos en 1
            root -> data = data_type();
            return;
        }
        int mi = (root -> l + root -> r) / 2;
        root -> left = new SegmentTreeNode(data_type(), root -> l, mi, nullptr, nullptr);
        root -> right = new SegmentTreeNode(data_type(), mi + 1, root -> r, nullptr, nullptr);
        build(root -> left);
        build(root -> right);
    }

    void build(SegmentTreeNode *root, vector<data_type> &a) {
        if (root -> l == root -> r) {
            // -1 porque indexamos en 1
            root -> data = a[root -> l - 1];
            return;
        }
        int mi = (root -> l + root -> r) / 2;
        root -> left = new SegmentTreeNode(data_type(), root -> l, mi, nullptr, nullptr);
        root -> right = new SegmentTreeNode(data_type(), mi + 1, root -> r, nullptr, nullptr);
        build(root -> left, a);
        build(root -> right, a);
    }

    void update(int pos, data_type value, SegmentTreeNode *last, SegmentTreeNode *curr) {
        if (curr -> l == curr -> r) {
            curr -> data = value;
            return;
        }
        int mi = (curr -> l + curr -> r) / 2;
        if (pos <= mi) {
            curr -> right = last -> right;
            curr -> left = new SegmentTreeNode(last -> left -> data, curr -> l, mi, nullptr, nullptr);
            update(pos, value, last -> left, curr -> left);
        }
        else {
            curr -> left = last -> left;
            curr -> right = new SegmentTreeNode(last -> right -> data, mi + 1, curr -> r, nullptr, nullptr);
            update(pos, value, last -> right, curr -> right);
        }
        curr -> data = curr -> left -> data + curr -> right -> data;
    }

    int update(int version, int pos, data_type value) {
        SegmentTreeNode *root = new SegmentTreeNode(data_type(), version_roots[0] -> l, version_roots[0] -> r, nullptr, nullptr);
        version_roots.emplace_back(root);
        update(pos, value, version_roots[version], root);
        return (int)version_roots.size() - 1;
    }

    data_type query(int x, int y, SegmentTreeNode *root) {
        if (y < root -> l or root -> r < x or x > y) return data_type(0);
        if (x <= root -> l and root -> r <= y) return root -> data;
        return query(x, y, root -> left) + query(x, y, root -> right);
    }

    data_type query(int version, int x, int y) {
        return query(x, y, version_roots[version]);
    }

    int get_current_version() {
        return (int)version_roots.size() - 1;
    }
};


int main() {
    cin.tie(0) -> sync_with_stdio(false);
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    PersistentSegmentTree<int> S(1, n, a);
    for (int s = 1; s <= q; ++s) {
        int op;
        cin >> op;
        if (op == 1) {
            int v, p, x;
            cin >> v >> p >> x;
            S.update(v, p, x);
        }
        else {
            int v, l, r;
            cin >> v >> l >> r;
            cout << S.query(v, l, r) << '\n';
        }
    }
    return 0;
}
//------------------------------------------------
// -------END PERSISTENT SEGMENT TREE--------------------------
//------------------------------------------------






//------------------------------------------------
// -------PERSISTENT STACK--------------------------
//------------------------------------------------

template<typename data_type>
struct PersistentStack {
    struct StackNode {
        data_type data;
        StackNode* next;
    };

    vector<StackNode*> version_roots;

    PersistentStack() {
        version_roots.push_back(nullptr);
    }

    void update_push(int version, data_type data) {
        version_roots.emplace_back(new StackNode(data, version_roots[version]));
    }

    void update_pop(int version) {
        version_roots.emplace_back(version_roots[version] -> next);
    }

    data_type top(int version) {
        return version_roots[version] == nullptr ? data_type(0) : version_roots[version] -> data;
    }

    void print(int version) {
        StackNode* top = version_roots[version];
        while (top != nullptr) {
            cout << top -> data << endl;
            top = top -> next;
        }
        cout << "END OF STACK" << endl;
    }
};

int main() {
    PersistentStack<int> S;
    int q;
    cin >> q;
    for (int i = 1; i <= q; ++i) {
        int t, m;
        cin >> t >> m;
        if (m == 0) {
            S.update_pop(t);
        }
        else {
            S.update_push(t, S.top(t) + m);
        }
    }
    long long res = 0;
    for (int i = 1; i <= q; ++i) res += S.top(i);
    cout << res << '\n';
    return 0;
}


//------------------------------------------------
// -------END PERSISTENT STACK--------------------------
//------------------------------------------------




//------------------------------------------------
// -------PERSISTENT TRIE-------------------------
//------------------------------------------------

template<int ALPHABET = 26>
struct PersistentTrie {

    struct TrieNode {
        int prefix_count;
        int word_count;
        TrieNode* child[ALPHABET];

        TrieNode() {
            prefix_count = 0;
            word_count = 0;

            for (int i = 0; i < ALPHABET; ++i)
                child[i] = nullptr;
        }

        TrieNode(TrieNode* other) {
            if (other == nullptr) {
                prefix_count = 0;
                word_count = 0;

                for (int i = 0; i < ALPHABET; ++i)
                    child[i] = nullptr;
            }
            else {
                prefix_count = other->prefix_count;
                word_count = other->word_count;

                for (int i = 0; i < ALPHABET; ++i)
                    child[i] = other->child[i];
            }
        }
    };

    vector<TrieNode*> version_roots;

    PersistentTrie() {
        version_roots.push_back(new TrieNode());
    }


    TrieNode* update(
        TrieNode* last,
        const string& word,
        int pos,
        int delta
    ) {
        TrieNode* curr = new TrieNode(last);

        curr->prefix_count += delta;

        // Terminamos la palabra
        if (pos == (int)word.size()) {
            curr->word_count += delta;
            return curr;
        }

        int c = word[pos] - 'a';

        TrieNode* old_child = nullptr;

        if (last != nullptr)
            old_child = last->child[c];

        curr->child[c] =
            update(old_child, word, pos + 1, delta);

        return curr;
    }


    int insert(int version, const string& word) {

        TrieNode* new_root =
            update(version_roots[version], word, 0, +1);

        version_roots.push_back(new_root);

        return (int)version_roots.size() - 1;
    }


    int count_word(int version, const string& word) {

        TrieNode* curr = version_roots[version];

        for (char ch : word) {

            int c = ch - 'a';

            if (curr == nullptr || curr->child[c] == nullptr)
                return 0;

            curr = curr->child[c];
        }

        return curr->word_count;
    }


    int count_prefix(int version, const string& prefix) {

        TrieNode* curr = version_roots[version];

        for (char ch : prefix) {

            int c = ch - 'a';

            if (curr == nullptr || curr->child[c] == nullptr)
                return 0;

            curr = curr->child[c];
        }

        return curr->prefix_count;
    }


    bool contains(int version, const string& word) {
        return count_word(version, word) > 0;
    }


    int get_current_version() {
        return (int)version_roots.size() - 1;
    }
};

int main() {
    PersistentTrie<26> T;
    int q;
    cin >> q;
    for (int s = 1; s <= q; ++s) {
        int op;
        cin >> op;
        if (op == 1) {
            int v;
            string word;
            cin >> v >> word;
            T.insert(v, word);
        }
        else {
            int v;
            string word;
            cin >> v >> word;
            cout << T.count_word(v, word) << '\n';
        }
    }
    return 0;
}

//--------------------------------------------------
// -------END PERSISTENT TRIE-------------------------
//--------------------------------------------------        



