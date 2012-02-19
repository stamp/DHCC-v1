<?php

class paging extends module {

    var $total = -1;
    var $start = 0;
    var $limit = 10;      // default post per page limit
    var $page  = 0; 
    var $spage = 0;       // default start page
    var $show_pages = 5; // how many pagelinks should be shown

    function __construct () {
        $this->setId(get_class($this));
        $this->setName('Paging');
        $this->setDescription('Creates a page function for multipost pages');
        $this->setVersion('0.1.0');
        $this->setRequiredCoreApi('1.0');
        $this->setRequiredModuleApi('1.0');
        $this->setMaintainer('Jonathan Svensson-Köhler <stamp@stamp.se>');
    }
    // {{{ make($total)
    /**
     * Initis a new paging setup and loads the get variables
     *
     **/
    function make($total) {
        if(!is_numeric($total))
            return false;
            
        $this->total = $total;

        if (isset($_GET['page'])&&is_numeric($_GET['page']))
            $this->page = $_GET['page'];
        
        $this->calculate();

        return true;
    }
    // }}}
    // {{{ calculate()
    function calculate() {  
        if ( !(isset($_GET['page'])&&is_numeric($_GET['page'])) )
            if ( !is_numeric($this->spage)&&$this->spage=='last' )
                $this->page = intval(($this->total-1)/$this->limit);
            else 
                $this->page = $this->spage;


        $this->start = $this->limit * $this->page;
    }
    // }}}
    // {{{ show()
    /**
     * Prints out the paging menu
     *
     **/
    function show() {
        global $tpl;

        if ($this->total<0) {
            send(E_ERROR,'Paging module is not initiated');
            return false;
        }
        
        if (!isset($this->paging)) {
            $this->paging = array();
            $page = 0;
            
            $this->paging[] = array(
                    'head' => 'Första',
                    'page' => 0,
                    'active' => ($this->page==0)
            );

            $prev = $this->page > 0 ? $this->page-1 : 0;

            $this->paging[] = array(
                    'head' => 'Föregående',
                    'page' => $prev,
                    'active' => ($this->page==$prev)
            );
            
            $pages = intval(($this->total-1)/$this->limit);

            if ($this->page >  intval($this->show_pages/2)) {
                $from = $this->page - intval($this->show_pages/2);
                $to = 0;
            } else {
                $to = (intval($this->show_pages/2) - $this->page);
                $from = 0;
            }

            if ($this->page <= intval($this->show_pages/2)) {
                $to += $this->page + intval($this->show_pages/2);
            } else {
                if ($from>1) 
                    $from -= intval($this->show_pages/2) - ($pages-$this->page);
                $to += $pages;
            }
            
            
            $page = $from;
            while ($page < ($this->total/$this->limit) && $page <= $to) {
                $this->paging[] = array(
                    'head' => $page+1,
                    'page' => $page,
                    'active' => ($this->page==$page)
                );
                $page++;
            }
            
            $page--;

            $next = $this->page < $page ? $this->page+1 : $page;

            $this->paging[] = array(
                    'head' => 'Nästa',
                    'page' => $next,
                    'active' => ($this->page==$next)
            );

            $this->paging[] = array(
                    'head' => 'Sista',
                    'page' => $pages,
                    'active' => ($this->page==$pages)
            );
        }

        $tpl->paging = $this->paging;

        $this->display('show.tpl');
    }
    // }}}
    // {{{ startpage() 
    function startpage ($start) {
        if ($this->total<0) {
            send(E_ERROR,'Paging module is not initiated');
            return false;
        }

        if ($start=='last') {
            $this->spage = $start;
            $this->calculate();
        }

        if (!( $start>=0 && $start<=($this->total/$this->limit) ))
            return false;

        $this->spage = $start;
        $this->calculate();
    }
}

?>
