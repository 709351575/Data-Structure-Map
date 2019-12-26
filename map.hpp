/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"
#include <iostream>
namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
>
class map {
public:
    typedef pair<const Key, T> value_type;
private:
    struct mapNode
    {
        value_type* data;
        mapNode* parent;
        mapNode* left;
        mapNode* right;
        int colour; //0--red,1--black
        mapNode()
        {
            data=NULL;
            parent=left=right=NULL;
        }
        mapNode(const Key* k)
        {
            T tmp;
            data=new value_type(*k,tmp);
            parent=left=right=NULL;
            colour=0;
        }
        mapNode(value_type *d, mapNode *p=NULL,mapNode *l=NULL,mapNode *r=NULL)
        {
            data=new value_type(d->first,d->second);
            parent=p;
            left=l;
            right=r;
            colour=0;
        }
        mapNode(const value_type *d, mapNode *p=NULL,mapNode *l=NULL,mapNode *r=NULL)
        {
            data=new value_type(d->first,d->second);
            parent=p;
            left=l;
            right=r;
            colour=0;
        }
    };

    mapNode *root;
    int currentSize;

    mapNode *copyNode(mapNode *q)
    {
        if(!q)
            return NULL;
        mapNode *p=new mapNode(q->data,q->parent,copyNode(q->left),copyNode(q->right));
        p->colour=q->colour;
        if(p->left)
            p->left->parent=p;
        if(p->right)
            p->right->parent=p;
        return p;
    }

    void LL(mapNode *&x)
    {
        mapNode *y=x->right;
        x->right=y->left;
        if(y->left)
            y->left->parent=x;
        y->parent=x->parent;
        if(!x->parent)
            root=y;
        else
        {
            if(x->parent->left==x)
                x->parent->left=y;
            else
                x->parent->right=y;
        }
        y->left=x;
        x->parent=y;
    }

    void LR(mapNode *&t,mapNode *&p)
    {
        RR(t->left,t);
        LL(t,p);
    }

    void RL(mapNode *&t,mapNode *&p)
    {
        LL(t->right,t);
        RR(t,p);
    }

    void RR(mapNode *&y)
    {
        mapNode *x=y->left;
        y->left=x->right;
        if(x->right)
            x->right->parent=y;
        x->parent=y->parent;
        if(!y->parent)
            root=x;
        else
        {
            if(y->parent->right==y)
                y->parent->right=x;
            else
                y->parent->left=x;
        }
        x->right=y;
        y->parent=x;
    }


    void makeEmpty(mapNode *&t)
    {
        if(t)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            if(t->data)
                delete t->data;
            delete t;
        }
        t=NULL;
    }

    void insertRebalance(mapNode *&t)
    {
        mapNode *father,*gfather;
        while((father=(t->parent))&&father->colour==0)
        {
            gfather=father->parent;
            if(!gfather)
                return;
            if(!gfather)
                return;
            if(father==gfather->left)
            {
                mapNode *uncle=gfather->right;
                if(uncle&&uncle->colour==0)
                {
                    uncle->colour=1;
                    father->colour=1;
                    gfather->colour=0;
                    t=gfather;
                    continue;
                }

                if(father->right==t)
                {
                    mapNode *tmp;
                    LL(father);
                    tmp=father;
                    father=tmp;
                    t=tmp;
                }
                father->colour=1;
                gfather->colour=0;
                RR(gfather);
            }
            else
            {
                mapNode *uncle=gfather->left;
                if(uncle&&uncle->colour==0)
                {
                    uncle->colour=1;;
                    father->colour=1;
                    gfather->colour=0;
                    t=gfather;
                    continue;
                }

                if(father->left==t)
                {
                    mapNode *tmp;
                    RR(father);
                    tmp=father;
                    father=t;
                    t=tmp;
                }
                father->colour=1;
                gfather->colour=0;
                LL(gfather);
            }
        }
        root->colour=1;
    }

    void removeReblance(mapNode *&node,mapNode *&father)
    {
        mapNode *other;
        while((!node||(node->colour==1))&&(node!=root))
        {
            if(father->left==node)
            {
                other=father->right;
                if(!other)
                    return;
                if(other->colour==0)
                {
                    other->colour=1;
                    father->colour=0;
                    LL(father);
                    other=father->right;
                }
                if(other&&((!other->left||(other->left->colour==1))
                    &&(!other->right||(other->right->colour==1))))
                {
                    other->colour=0;
                    node=father;
                    father=node->parent;
                }
                else
                {
                    if(other&&(!other->right||(other->right->colour==1)))
                    {
                        other->left->colour=1;
                        other->colour=0;
                        RR(other);
                        other=father->right;
                    }
                    if(other)
                    {
                        other->colour=father->colour;
                        father->colour=1;
                        other->right->colour=1;
                        LL(father);
                        node =root;
                        break;
                    }

                }
            }
            else
            {
                other=father->left;
                if(!other)
                    return;
                if(other->colour==0)
                {
                    other->colour=1;
                    father->colour=0;
                    RR(father);
                    other=father->left;
                }

                if(other&&((!other->left||other->left->colour==1)&&
                   (!other->right||other->right->colour==1)))
                {
                    other->colour=0;
                    node=father;
                    father=node->parent;
                }
                else
                {
                    if(other&&(!other->left||other->left->colour))
                    {
                        other->right->colour=1;
                        other->colour=0;
                        LL(other);
                        other=father->left;
                    }
                    if(other)
                    {
                        other->colour=father->colour;
                        father->colour=1;;
                        other->left->colour=1;
                        RR(father);
                        node=root;
                        break;
                    }

                }
            }
        }
        if(node)
            node->colour=1;
    }

public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */


	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	    friend class const_iterator;
	    friend class map;
	private:
	    mapNode *root;
	    mapNode *it;
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		iterator()
		{
			root=NULL;
			it=NULL;
		}
		iterator(mapNode *r,mapNode *i)
		{
		    root=r;
		    it=i;
		}
		iterator(const iterator &other)
		{
			root=other.root;
			it=other.it;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		 /*/
		 如果n只有左孩子或者n没有孩子，向上找到第一个节点k使得k是他的parent的左孩子，指向k的parent，返回，如果找不到，那就是遍历结束
            如果n有右孩子，前进到右孩子，然后向左找到尽头
		 /*/
		iterator operator++(int)
		{
		    iterator tmp(it,root);
            if(!root||!it)
                throw invalid_iterator();
            if(it->right)
            {
                it=it->right;
                while(it->left)
                    it=it->left;
            }
            else
            {
                mapNode* p=it->parent;
                while(p&&p->right==it)
                {
                    it=p;
                    p=p->parent;
                }
                it=p;
            }
            return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++()
		{
            if(!root||!it)
                throw index_out_of_bound();
            if(it->right)
            {
                it=it->right;
                while(it->left)
                    it=it->left;
            }
            else
            {
                mapNode* p=it->parent;
                while(p&&p->right==it)
                {
                    it=p;
                    p=p->parent;
                }
                it=p;
            }
            return *this;
		}
		/**
		 * TODO iter--
		 */
		 mapNode *minNode(mapNode *root)
		 {
		     if(!root)
                return NULL;
             mapNode *p=root;
             while(p->left)
                p=p->left;
             return p;
		 }
		iterator operator--(int)
        {
            iterator tmp(root,it);
            if(it==minNode(root))
                throw index_out_of_bound();
            if(!it)
            {
                if(!root)
                    it=NULL;
                else
                {
                    it=root;
                    while(it->right)
                        it=it->right;
                }
                return tmp;
            }
            if(it->left)
            {
                it=it->left;
                while(it->right)
                    it=it->right;
                return tmp;
            }
            else
            {
                mapNode *p=it->parent;
                while(p&&p->right!=it)
                {
                    it=p;
                    p=p->parent;
                }
                it=p;
                return tmp;
            }
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--()
        {
            if(it==minNode(root))
                throw index_out_of_bound();
            if(it==NULL)
            {
                if(!root)
                    it=NULL;
                else
                {
                    it=root;
                    while(it->right)
                        it=it->right;
                }
                return *this;
            }
            if(it->left)
            {
                mapNode *p=it->left;
                while(p->right)
                    p=p->right;
                it=p;
                return *this;
            }
            else
            {
                mapNode *p=it->parent;
                while(p&&p->right!=it)
                {
                    it=p;
                    p=p->parent;
                }
                it=p;
                return *this;
            }
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const
		{
		    if(!it)
                throw invalid_iterator();
            return *it->data;
		}
		bool operator==(const iterator &rhs) const
		{
		    return (root==rhs.root&&it==rhs.it);
		}
		bool operator==(const const_iterator &rhs) const
        {
		    return (root==rhs.root&&it==rhs.it);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
        {
		    return (root!=rhs.root||it!=rhs.it);
		}
		bool operator!=(const const_iterator &rhs) const
        {
		    return (root!=rhs.root||it!=rhs.it);
		}
		/**
		 * for the support of it->first.
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept
		{
		    if(!root||!it)
                return NULL;
            else
                return it->data;
		}
	};
	class const_iterator {
            friend class iterator;
            friend class map;
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
		    // data members.
            mapNode *root;
            mapNode *it;
		public:
			const_iterator()
			{
				root=NULL;
				it=NULL;
			}
            const_iterator(mapNode *r,mapNode *i)
            {
                root=r;
                it=i;
            }
			const_iterator(const const_iterator &other)
			{
				root=other.root;
				it=other.it;
			}
			const_iterator(const iterator &other)
			{
				root=other.root;
				it=other.it;
			}

            const_iterator operator++(int)
            {
                const_iterator tmp(it,root);
                if(!root||!it)
                    throw invalid_iterator();
                if(it->right)
                {
                    it=it->right;
                    while(it->left)
                        it=it->left;
                }
                else
                {
                    mapNode* p=it->parent;
                    while(p&&p->right==it)
                    {
                        it=p;
                        p=p->parent;
                    }
                    it=p;
                }
                return tmp;
            }
            /**
             * TODO ++iter
             */
            const_iterator & operator++()
            {
                if(!root||!it)
                    throw index_out_of_bound();
                if(it->right)
                {
                    it=it->right;
                    while(it->left)
                        it=it->left;
                }
                else
                {
                    mapNode* p=it->parent;
                    while(p&&p->right==it)
                    {
                        it=p;
                        p=p->parent;
                    }
                    it=p;
                }
                return *this;
            }
            /**
             * TODO iter--
             */
             mapNode *minNode(mapNode *root)
             {
                 if(!root)
                    return NULL;
                 mapNode *p=root;
                 while(p->left)
                    p=p->left;
                 return p;
             }
            const_iterator operator--(int)
            {
                const_iterator tmp(root,it);
                if(it==minNode(root))
                    throw index_out_of_bound();
                if(!it)
                {
                    if(!root)
                        it=NULL;
                    else
                    {
                        it=root;
                        while(it->right)
                            it=it->right;
                    }
                    return tmp;
                }
                if(it->left)
                {
                    it=it->left;
                    while(it->right)
                        it=it->right;
                    return tmp;
                }
                else
                {
                    mapNode *p=it->parent;
                    while(p&&p->right!=it)
                    {
                        it=p;
                        p=p->parent;
                    }
                    it=p;
                    return tmp;
                }
            }
            /**
             * TODO --iter
             */
            const_iterator & operator--()
            {
                if(it==minNode(root))
                    throw index_out_of_bound();
                if(!it)
                {
                    if(!root)
                        it=NULL;
                    else
                    {
                        it=root;
                        while(it->right)
                            it=it->right;
                    }
                    return *this;
                }
                if(it->left)
                {
                    it=it->left;
                    while(it->right)
                        it=it->right;
                    return *this;
                }
                else
                {
                    mapNode *p=it->parent;
                    while(p&&p->right!=it)
                    {
                        it=p;
                        p=p->parent;
                    }
                    it=p;
                    return *this;
                }
            }
            value_type & operator*() const
            {
                if(!it)
                    throw invalid_iterator();
                return *it->data;
            }
            bool operator==(const iterator &rhs) const
            {
                return (root==rhs.root&&it==rhs.it);
            }
            bool operator==(const const_iterator &rhs) const
            {
                return (root==rhs.root&&it==rhs.it);
            }

            bool operator!=(const iterator &rhs) const
            {
                return (root!=rhs.root||it!=rhs.it);
            }
            bool operator!=(const const_iterator &rhs) const
            {
                return (root!=rhs.root||it!=rhs.it);
            }
            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type* operator->() const noexcept
            {
                return it->data;
            }
	};
	/**
	 * TODO two constructors
	 */
	map()
	{
	    root=NULL;
        currentSize=0;
	}
	map(const map &other)
	{
        root=copyNode(other.root);
        currentSize=other.currentSize;
	}

	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other)
	{
	    if(this==&other)
            return *this;
        clear();
        if(root)
        {
            delete root->data;
            delete root;
        }
        root=copyNode(other.root);
        currentSize=other.currentSize;
        return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map()
	{
        currentSize=0;
        makeEmpty(root);
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */

	T & at(const Key &key)
	{
	    Compare cmp;
        mapNode *p=root;
        while(p)
        {
            Key keytmp=p->data->first;
            if(cmp(key,keytmp))
                p=p->left;
            else
            {
                if(cmp(keytmp,key))
                    p=p->right;
                else
                    break;
            }
        }
        if(p)
            return p->data->second;
        else
            throw index_out_of_bound();
	}

	const T & at(const Key &key) const
	{
	    Compare cmp;
        mapNode *p=root;
        while(p)
        {
            Key keytmp=p->data->first;
            if(cmp(key,keytmp))
                p=p->left;
            else
            {
                if(cmp(keytmp,key))
                    p=p->right;
                else
                    break;
            }
        }
        if(p)
            return p->data->second;
        else
            throw index_out_of_bound();
	}

	/**
	 * TODO
	 * access specified element
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */

	T & operator[](const Key &key) //insert
    {
        iterator x=find(key);
        if(x==end())
        {
            T tmp;
            value_type value(key,tmp);
            x=insert(value).first;
        }
        return x.it->data->second;
	}


	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */

	const T & operator[](const Key &key) const //insert
	{
	    Compare cmp;
        mapNode *p=root;
        while(p)
        {
            Key keytmp=p->data->first;
            if(cmp(key,keytmp))
                p=p->left;
            else
            {
                if(cmp(keytmp,key))
                    p=p->right;
                else
                    break;
            }
        }
        if(p)
            return p->data->second;
        else
            throw index_out_of_bound();
	}

	/**
	 * return a iterator to the beginning
	 */
	iterator& begin()
	{
	    if(!root)
        {
            iterator *it=new iterator(NULL,NULL);
            return *it;
        }
        if(!root->left)
        {
            iterator *it=new iterator(root,root);
            return *it;
        }
	    mapNode *p=root;
	    while(p->left)
            p=p->left;
	    iterator *it=new iterator(root,p);
	    return *it;
	}
	const_iterator& cbegin() const
	{
	    if(!root)
        {
            const_iterator *it=new const_iterator(NULL,NULL);
            return *it;
        }
        if(!root->left)
        {
            const_iterator *it=new const_iterator(root,root);
            return *it;
        }
	    mapNode *p=root;
	    while(p->left)
            p=p->left;
	    const_iterator *it=new const_iterator(root,p);
	    return *it;
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator& end()
	{
        iterator *tmp=new iterator(root,NULL);
        return *tmp;
	}
	const_iterator& cend() const
	{
        const_iterator *tmp=new const_iterator(root,NULL);
        return *tmp;
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const
	{
	    return (!currentSize);
	}
	/**
	 * returns the number of elements.
	 */
    size_t size() const
	{
	    return currentSize;
	}
	/**
	 * clears the contents
	 */
	void clear()
	{
        makeEmpty(root);
        currentSize=0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion),
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool>& insert(const value_type &value)
	{
        mapNode *y=NULL;
        mapNode *x=root;
        Key key=value.first;
        Compare cmp;
        while(x)
        {
            y=x;
            if(cmp(key,x->data->first))
            {
                x=x->left;
            }
            else
                if(cmp(x->data->first,key))
                {
                    x=x->right;
                }
                else
                {
                    iterator *tmp=new iterator(root,x);
                    pair<iterator, bool> *mypair=new pair<iterator,bool>(*tmp,0);
                    return *mypair;
                }
        }
        x=new mapNode(&value,y,NULL,NULL);
        if(y)
        {
            if(cmp(x->data->first,y->data->first))
                y->left=x;
            else
                y->right=x;
        }
        else
            root=x;
        x->colour=0;
        iterator *tmp=new iterator(root,x);
        pair<iterator, bool> *mypair=new pair<iterator,bool>(*tmp,1);
        ++currentSize;
        insertRebalance(x);
        return *mypair;
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	 void remove(mapNode *&node)
	 {
	     mapNode *child,*father;
	     int color;
	     if(node->left&&node->right)
        {

             mapNode *replace =node;
             replace=replace->right;
             while(replace->left)
                replace=replace->left;
            delete node->data;
            node->data=NULL;
            node->data=new value_type(*replace->data);
            return remove(replace);
         }

         if(node->left)
            child=node->left;
         else
            child=node->right;
         father=node->parent;
         color=node->colour;
         if(child)
            child->parent=father;
         if(father)
         {
             if(father->left==node)
                father->left=child;
             else
                father->right=child;
         }
         else
            root=child;
         if(color)
            removeReblance(child,father);
         delete node->data;
         delete node;
	 }
	void erase(iterator pos)
	{
	    if(pos.root!=root)
            throw invalid_iterator();
		if (!pos.it)
			throw invalid_iterator();

        iterator tmp=find(pos.it->data->first);
       if(tmp==end())
            return;
       remove(tmp.it);
       --currentSize;
	}
	/**
	 * Returns the number of elements with key
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const
	{
	    Compare cmp;
        mapNode *p=root;
        while(p)
        {
            if(!p->data)
                return 0;
            const Key keytmp=p->data->first;
            if(cmp(key,keytmp))
                p=p->left;
            else
            {
                if(cmp(keytmp,key))
                    p=p->right;
                else
                    return 1;
            }
        }
        return 0;
	}

	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */

	iterator& find(const Key &key)
	{
	    Compare cmp;
        mapNode *p=root;
        if(!root)
        {
            iterator *tmp=new iterator(NULL,NULL);
            return *tmp;
        }
        while(p)
        {
            if(!p->data)
            {
                iterator *it=new iterator(root,NULL);
                return *it;
            }
            const Key keytmp=p->data->first;
            if(cmp(key,keytmp))
            {
                p=p->left;
            }
            else
            {
                if(cmp(keytmp,key))
                {
                    p=p->right;
                }
                else
                {
                    iterator *it=new iterator(root,p);
                    return *it;
                }
            }
        }
        return end();
	}

	const_iterator& find(const Key &key) const
	{
	    Compare cmp;
        mapNode *p=root;
        while(p)
        {
            if(!p->data)
            {
                const_iterator *it=new const_iterator(root,NULL);
                return *it;
            }
            const Key keytmp=p->data->first;
            if(cmp(key,keytmp))
            {
                p=p->left;
            }
            else
            {
                if(cmp(keytmp,key))
                {
                    p=p->right;
                }
                else
                {
                    const_iterator *it=new const_iterator(root,p);
                    return *it;
                }
            }
        }
        return cend();
	}

    void preOrder(mapNode *p)
    {
        if(p)
        {
            preOrder(p->left);
            preOrder(p->right);
        }

    }

    void preOrder()
    {
        if(root)
        {
            preOrder(root->left);
            preOrder(root->right);
        }

    }
};

}

#endif
