-- ThePrimeagen-inspired visual-only Neovim configuration.
-- Source: https://github.com/ThePrimeagen/neovimrc

vim.g.mapleader = " "
vim.g.maplocalleader = " "

-- Only options that affect the editor's appearance.
vim.opt.termguicolors = true
vim.opt.guicursor = ""
vim.opt.number = true
vim.opt.relativenumber = true
vim.opt.wrap = false
vim.opt.scrolloff = 8
vim.opt.signcolumn = "yes"
vim.opt.colorcolumn = "80"

local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not (vim.uv or vim.loop).fs_stat(lazypath) then
  local output = vim.fn.system({
    "git",
    "clone",
    "--filter=blob:none",
    "--branch=stable",
    "https://github.com/folke/lazy.nvim.git",
    lazypath,
  })

  if vim.v.shell_error ~= 0 then
    vim.api.nvim_echo({
      { "Could not install lazy.nvim:\n", "ErrorMsg" },
      { output, "WarningMsg" },
    }, true, {})
    return
  end
end
vim.opt.rtp:prepend(lazypath)

local function transparent_background()
  vim.api.nvim_set_hl(0, "Normal", { bg = "none" })
  vim.api.nvim_set_hl(0, "NormalFloat", { bg = "none" })
end

require("lazy").setup({
  -- The active colorscheme in ThePrimeagen's config.
  {
    "rose-pine/neovim",
    name = "rose-pine",
    lazy = false,
    priority = 1000,
    opts = {
      variant = "main",
      styles = { transparency = true },
    },
    config = function(_, opts)
      require("rose-pine").setup(opts)
      vim.cmd.colorscheme("rose-pine")
      transparent_background()
    end,
  },

  -- Also present upstream; switch with :colorscheme tokyonight-storm.
  {
    "folke/tokyonight.nvim",
    lazy = true,
    opts = {
      style = "storm",
      transparent = true,
      terminal_colors = true,
      styles = {
        comments = { italic = false },
        keywords = { italic = false },
        sidebars = "dark",
        floats = "dark",
      },
    },
  },

  {
    "nvim-treesitter/nvim-treesitter",
    branch = "master",
    lazy = false,
    build = ":TSUpdate",
    config = function()
      require("nvim-treesitter.configs").setup({
        ensure_installed = {
          "bash",
          "c",
          "javascript",
          "jsdoc",
          "lua",
          "rust",
          "typescript",
          "vimdoc",
        },
        auto_install = true,
        highlight = {
          enable = true,
          additional_vim_regex_highlighting = { "markdown" },
        },
      })
    end,
  },

  {
    "nvim-telescope/telescope.nvim",
    cmd = "Telescope",
    dependencies = { "nvim-lua/plenary.nvim" },
    opts = {},
    keys = {
      { "<leader>pf", "<cmd>Telescope find_files<cr>", desc = "Find files" },
      { "<C-p>", "<cmd>Telescope git_files<cr>", desc = "Find Git files" },
      { "<leader>pws", "<cmd>Telescope grep_string<cr>", desc = "Search word" },
      { "<leader>ps", "<cmd>Telescope live_grep<cr>", desc = "Live grep" },
      { "<leader>vh", "<cmd>Telescope help_tags<cr>", desc = "Help tags" },
    },
  },

  {
    "folke/trouble.nvim",
    cmd = "Trouble",
    opts = { icons = false },
    keys = {
      { "<leader>tt", "<cmd>Trouble diagnostics toggle<cr>", desc = "Diagnostics" },
      { "[t", "<cmd>Trouble next jump=true<cr>", desc = "Next Trouble item" },
      { "]t", "<cmd>Trouble prev jump=true<cr>", desc = "Previous Trouble item" },
    },
  },

  {
    "mbbill/undotree",
    cmd = "UndotreeToggle",
    keys = {
      { "<leader>u", "<cmd>UndotreeToggle<cr>", desc = "Undo tree" },
    },
  },

  {
    "folke/zen-mode.nvim",
    cmd = "ZenMode",
    keys = {
      {
        "<leader>zz",
        function()
          require("zen-mode").toggle({
            window = {
              width = 90,
              options = {
                number = true,
                relativenumber = true,
                wrap = false,
              },
            },
            on_open = transparent_background,
          })
        end,
        desc = "Zen Mode",
      },
      {
        "<leader>zZ",
        function()
          require("zen-mode").toggle({
            window = {
              width = 80,
              options = {
                colorcolumn = "0",
                number = false,
                relativenumber = false,
                wrap = false,
              },
            },
            on_open = transparent_background,
          })
        end,
        desc = "Zen Mode (minimal)",
      },
    },
    opts = {},
  },

  {
    "laytan/cloak.nvim",
    event = "BufReadPre",
    opts = {
      enabled = true,
      cloak_character = "*",
      highlight_group = "Comment",
      patterns = {
        {
          file_pattern = { ".env*", "wrangler.toml", ".dev.vars" },
          cloak_pattern = "=.+",
        },
      },
    },
  },

  {
    "Eandrju/cellular-automaton.nvim",
    cmd = "CellularAutomaton",
    keys = {
      {
        "<leader>mr",
        "<cmd>CellularAutomaton make_it_rain<cr>",
        desc = "Make it rain",
      },
    },
  },
}, {
  change_detection = { notify = false },
  install = { colorscheme = { "rose-pine", "habamax" } },
})

vim.api.nvim_create_autocmd("ColorScheme", {
  callback = transparent_background,
})
